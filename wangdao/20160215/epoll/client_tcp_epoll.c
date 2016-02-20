#include "func.h"

#define NUM 10
int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
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
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	int epfd;
	epfd=epoll_create(1);
	if(-1==epfd)
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev,evs[2];
	ev.events=EPOLLIN;//注册前先定义动作
	ev.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);//注册sfd	
	if(-1==ret)
	{
		perror("epoll_ctl1");
		return -1;
	}
	ev.events=EPOLLIN;
	ev.data.fd=0;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&ev);//注册标准输入
	if(-1==ret)
	{
		perror("epoll_ctl2");
		return -1;
	}
	int i;
	char buf[128];
	int size;
	while(1)
	{
		bzero(evs,sizeof(evs));
		ret=epoll_wait(epfd,evs,2,-1);//监控哪个可读
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
		   	if(evs[i].data.fd == 0 && evs[i].events == EPOLLIN)
				{
					bzero(buf,sizeof(buf));
					read(0,buf,sizeof(buf));
					size=send(sfd,buf,strlen(buf)-1,0);
					if(-1==size)
					{
					}
				}
				if(evs[i].data.fd == sfd && evs[i].events == EPOLLIN)
				{
					bzero(buf,sizeof(buf));
					size=recv(sfd,buf,sizeof(buf),0);
					if(size > 0)
					{
						printf("%s\n",buf);
					}else{
						close(sfd);
						return 0;
					}
				}
			}
		}	
	}
	close(sfd);
	return 0;
}			 		

