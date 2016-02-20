#include "head.h"

int main(int argc,char* argv[])//IP PORT 进程数目
{
	if(argc !=4)
	{
		printf("error args\n");
		return -1;
	}
	pchild cptr;//定义结构体指针，记录子进程信息
	int num=atoi(argv[3]);//进程数
	cptr=(pchild)calloc(num,sizeof(child));//存放子进程信息数据结构，有num个
	make_child(cptr,num);//往结构体填pid,sockpair的描述符，忙碌标志
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);//进行socket通信
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
	listen(sfd,num);
	int epfd;
	epfd=epoll_create(1);//产生epoll多路复用句柄epfd
	if(-1==epfd)
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev,*evs;//evs数组不能是常量，因为num是动态的，因此定义为指针
	evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
	//监控每个soketpair的读端和本身的sfd,因此申请num+1个空间
	ev.events=EPOLLIN;
	ev.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);	
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	int i;
	for(i=0;i<num;i++)//用循环来添加sockpair的读端
	{
		evs[i].events=EPOLLIN;
		evs[i].data.fd=cptr[i].fds;//cptr是结构体指针，采用下标法，只需要监控对应的写端，这里写时epoll_wait不会响应，wait当对端可读时才会响应，这里是写
		ret=epoll_ctl(epfd,EPOLL_CTL_ADD,cptr[i].fds,&evs[i]);	
		if(-1==ret)
		{
			perror("epoll_ctl1");
			return -1;
		}
	}
	int j;
	int new_fd;
	int flag;
	int size;
	while(1)
	{
		ret=epoll_wait(epfd,evs,num+1,-1);	
		if(ret>0)//说明有返回
		{
			for(i=0;i<ret;i++)//循环判断是哪个
			{
				if(evs[i].data.fd == sfd && evs[i].events == EPOLLIN)//触发时evs[i].events会被置位
				{
					new_fd=accept(sfd,NULL,NULL);
					if(-1==new_fd)
					{
						perror("accept");	
						return -1;
					}
					printf("one client connect,new_fd is %d\n",new_fd);
					for(j=0;j<num;j++)//找空闲的子进程
					{
						if(cptr[j].busy == 0)
						{
							break;//找到后跳出
						}
					}
					if(j !=num)
					{
						send_fd(cptr[j].fds,new_fd);//将new_fd给对应子进程，标示为忙碌
						cptr[j].busy =1;//发给谁之后标为忙碌
						printf("child %d is busy\n",j);
					}
				}
				for(j=0;j<num;j++)//遍历是哪个子进程发送数据过来，代表对方不忙碌
				{
					if(evs[i].data.fd == cptr[j].fds && evs[i].events == EPOLLIN)
					{
						size=read(cptr[j].fds,&flag,4);
						//如果这里没有将数据读取出去，则epoll_wait会一直触发，
						//即一直执行这段程序(不断打印)，因为epoll用的是水平触发	
						printf("the size is %d\n",size);//看size是不是4个字节，因为对端就写了四个字节
						cptr[j].busy=0;
						printf("child %d is not busy\n",j);
					}
				}
			}
		}	
	}
	return 0;	
}	
	
	

