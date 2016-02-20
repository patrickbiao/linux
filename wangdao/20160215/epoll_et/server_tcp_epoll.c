#include "func.h"

#define NUM 10
void change_nonblock(int* fd)//由于是fd被F_GETFL作用，所以传地址
{
	int status;
	status=fcntl(*fd,F_GETFL);//解引用,得fd状态
	status=status|O_NONBLOCK;//将状态设置为非阻塞的
	int ret;
	ret=fcntl(*fd,F_SETFL,status);//设置fd状态
	if(-1==ret)
	{
		perror("fcntl");
		return ;
	}

}

int main(int argc,char **argv)//要传IP地址和端口号
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	//进行tcp通信，1.首先要得到一个socket描述符
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	//2.绑定端口号和IP,所以首先要定义一个结构体
	struct sockaddr_in ser;//最新的这个多一个_in
	bzero(&ser,sizeof(ser));
	//赋值初始化
	ser.sin_family=AF_INET;//ipv4
	ser.sin_port = htons(atoi(argv[2]));//端口号
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));//为与旧版本兼容，强转成(struct sockaddr*)
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,NUM);//网络通信的句柄sfd本身是要监控的对象,虽然NUM定义了10，但这里只监听一个
	int epfd;
	epfd=epoll_create(1);//得到epoll句柄
	if(-1==epfd)	
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev,evs[3];//3个是标准输入，客户端新产生的newfd，还有sfd，ev用于告诉内核需要监听什么事，而数组evs用于注册事件个数，在wait中要用到
	ev.events=EPOLLIN;//对应的文件描述符可读
	ev.data.fd=sfd;//这里是定义sfd的动作
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);//注册sfd	
	if(-1==ret)
	{
		perror("epoll_ctl1");
		return -1;
	}
	ev.events=EPOLLIN;
	ev.data.fd=0;//监控标准输入
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&ev);//注册0
	if(-1==ret)
	{
		perror("epoll_ctl12");
		return -1;
	}
	int i;
	int new_fd=0;
	int size;
	char buf[128];
	while(1)
	{
/*while(1)通过epoll_wait一旦发现sfd可读，这时就需要accept,这时就会产生一个newfd,接着recv newfd时会卡住，所以在这里要重新使用epoll_ctl去添加newfd*/
		bzero(evs,sizeof(evs));//里边的长度为3个结构体长度
		ret=epoll_wait(epfd,evs,3,-1);//3为前面数组的长度
		change_nonblock(&new_fd);
		if(ret>0)//如果有可读，任何一个可读，都要返回全部
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].data.fd==sfd && evs[i].events==EPOLLIN)//如果触发就绪，events会被置
				{
					new_fd=accept(sfd,NULL,NULL);
					if(-1==new_fd)
					{
						perror("accept");
						return -1;
					}
					//接收完之后进行注册
					ev.events=EPOLLIN;
					ev.data.fd=new_fd;
					epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&ev);
				}
				if(new_fd>0);//send之前要判断客户端是否连接成功
				{
					/*标准输入可读*/			
					if(evs[i].data.fd==0 && evs[i].events==EPOLLIN)//如果触发就绪，events会被置
					{
						bzero(buf,sizeof(buf));
						read(0,buf,sizeof(buf));
						size=send(new_fd,buf,strlen(buf)-1,0);//ret在上面循环被用了
						if(-1==size)//send失败,对方断开时返回-1
						{
							perror("send");
							return -1;
						}
					}
				  
					/*new_fd可读*/
					if(evs[i].data.fd==new_fd && evs[i].events==EPOLLIN)//如果触发就绪，events会被置
					{
						while(1)//如果没有设置new_fd非阻塞，程序会被recv卡住，则不能响应标准输入的数据
						{
							bzero(buf,sizeof(buf));
							size=recv(new_fd,buf,3,0);
							if(size > 0)
							{
							//	printf("%s\n",buf);
								printf("%s",buf);
							}else if(size==-1){//非阻塞时读到最后时返回-1
									break;
							}else{//对客户端异常断开的处理，对其进行解注册
								ev.events=EPOLLIN;
								ev.data.fd=new_fd;
								epoll_ctl(epfd,EPOLL_CTL_DEL,new_fd,&ev);
								new_fd=0;//为了不让它进来
							}
						}	
						printf("\n");//break之后才刷新缓冲区，从而显示出来
					}

				}
			}
		}
	}
	close(sfd);
	return 0;
}
