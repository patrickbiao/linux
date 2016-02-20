#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);//客户端也需要生成网络通信的句柄，
                 							 //设置为ipv4,tcp通信
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	//将上面socket生成的sfd连到指定的服务器IP和端口号上去。
	if(-1==ret)
	{
		perror("connect");
		close(sfd);
		return -1;
	}
	printf("connect server success\n");
	char buf[128];
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);//对于客户端，不会产生newfd，只有两个阻塞：0和socket句柄sfd
		if(ret>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				ret=send(sfd,buf,strlen(buf)-1,0);
				if(-1==ret)
				{
					perror("send");
					close(sfd);
					return -1;
				}
			}
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret >0)
				{
					printf("I am client,the message is %s\n",buf);
				}
			}
		}
	}
	close(sfd);
	return 0;
}

