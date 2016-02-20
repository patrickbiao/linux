#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
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
	ret=connect(sfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));//客户端连接server
	if(-1==ret)
	{
		perror("connect");
		close(sfd);
		return -1;
	}
	printf("connect server success\n");
	char buf[128];
	bzero(buf,sizeof(buf));
	strcpy(buf,"how are you");
	ret=send(sfd,buf,strlen(buf),0);
	if(-1==ret)
	{
		perror("send");
		close(sfd);
		return -1;
	}
	bzero(buf,sizeof(buf));
	ret=recv(sfd,buf,sizeof(buf),0);
	if(ret >0)
	{
		printf("I am client,the message is %s\n",buf);
	}
	close(sfd);
	return 0;
}

