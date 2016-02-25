#include "factory.h"

int main(int argc,char* argv[])//主线程函数流程
{
	if(argc !=5)//传的参数IP PORT NUM QUE_CAPACITY
	{
		printf("error args\n");
		return -1;
	}
	int thread_num=atoi(argv[3]);
	int capacity=atoi(argv[4]);
	factory tf;//定义一个线程工厂的结构体，
	factory_init(&tf,thread_num,capacity,thread_handle);//线程工厂，线程数，能力，线程函数，
	//先初始再启动，放在factory.c中,主线程中调用数据准备、启动等，主线程主要是写epoll的逻辑
	factory_start(&tf);//传地址不仅为了修改数据，也为了节省空间。
	//地址只有四个字节，减少栈空间的开销，这里也传结构体进去，但栈空间浪费很大
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;	
	ret=bind(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,capacity);//listen的个数与队列中描述符个数相同
	int epfd=epoll_create(1);//创建epoll句柄
	struct epoll_event ev,evs[2];
	ev.events=EPOLLIN;
	ev.data.fd=sfd;//这里主要监控的就是sfd，因为子线程与客户端通信完之后是直接去队列中读而不是通知主线程
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev); 
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	int new_fd;
	while(1)
	{
		ret=epoll_wait(epfd,evs,2,-1);
		if(ret >0)//代表有客户端连接
		{
			if(evs[0].data.fd==sfd && evs[0].events == EPOLLIN)
			{
				new_fd=accept(sfd,NULL,NULL);//如果监控到有请求就接受，接着将new_fd放到队列中，即要写一系列函数
				pnode pnew;
				pnew=(pnode)calloc(1,sizeof(node));//要给指针申请空间之后才能赋值，
				//calloc默认申请的空间全是null ，如果用malloc，则要先初始化（赋值）null
				pnew->new_fd=new_fd;
				factory_que_set(&tf.fd_que,pnew);//将队列操作写到work_que.c中,主线程在放之前，
				//子线程都处于等待状态，因为前面已经调用子线程流程函数将子线程创建好了
				pthread_cond_signal(&tf.cond);//主线程每往队列中放一个，都signal一下
			}
		}
	}	
}

void* thread_handle(void* arg)//子线程函数流程，先初始化，再启动，一般这两个是分开的
{
	while(1)//让每个线程可以启动
	{
		pfactory pf=(pfactory)arg;
		printf("the start_flag is %d\n",pf->start_flag);
		pque_t pq=&pf->fd_que;
		pnode pcur;//因为中间没有对pcur进行判断，所以不用先初始化为null
		if(factory_que_empty(pq))//子线程判断队列是否为空，如果为空，则不去get。为空返回1，不为空返回0
		{
			pthread_cond_wait(&pf->cond,&pq->mutex);//wait之前得先加锁，wait时解锁，wait后又加锁，
			//为空等待对应的条件变量
			pthread_mutex_unlock(&pq->mutex);
		}
		factory_que_get(pq,&pcur);//等待结束又可以get
		send_file(pcur->new_fd);//将描述符发过去就可以向客户端发文件了
		free(pcur);//发过去之后将链表中该节点free掉,因为空间是在主线程中申请的，所以要在主线程中free				
	}
}	
void send_file(int sfd)
{
	int ret;
	data_t buf;
	buf.len=strlen(DOWN_FILE);
	strcpy(buf.buf,DOWN_FILE);
	ret=send(sfd,&buf, buf.len+4,0);//发送文件名	
	if(-1==ret)
	{
		perror("send1");
		return; 
	}
	int fd=open(DOWN_FILE,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	while(bzero(&buf,sizeof(buf)),(buf.len=read(fd,buf.buf,sizeof(buf.buf))) > 0)
	{
		send_n(sfd,(char*)&buf,buf.len+4);		
	}
	bzero(&buf,sizeof(buf));
	int flag=0;
	buf.len=sizeof(int);
	memcpy(buf.buf,&flag,4);
	send(sfd,&buf, buf.len+4,0);//发送结束符	
	close(sfd);
	return;
}

