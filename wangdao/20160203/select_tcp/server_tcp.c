#include "func.h"
#define NUM 10
int main(int argc,char* argv[])
{
	if(argc !=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络通信的描述符sfd
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
	//将点分十进制转化成32位二进制
	int ret;
	ret=bind(sfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));	
	if(-1==ret)
	{
		perror("bind");
		close(sfd);
		return -1;
	}
	ret=listen(sfd,NUM);//设定客户端连接的数目NUM
	if(-1==ret)
	{
		perror("listen");
		close(sfd);
		return -1;
	}
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));
	int length=sizeof(client_addr);
	int newfd=-1;//如果初始化为0，当没有accept时，变成了标准输入
	char buf[128];
	fd_set rdset;
	printf("sfd is %d\n",sfd);
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);//设置sfd(socket生成的网络通信描述符)到rdset集合
		//这时还没有newfd
		if(newfd >0)
		{
			FD_SET(newfd,&rdset);
		}
		ret=select(NUM+4,&rdset,NULL,NULL,NULL);//max为监听的客户端个数
      //加3个标准描述符加1，一直在这等，当可读时，ret大于0
		if(ret >0)
		{
			if(FD_ISSET(sfd,&rdset))
			{
				newfd=accept(sfd,(struct sockaddr*)&client_addr,&length);
				//三次握手在这瞬间完成，这里处于阻塞状态
				if(-1==newfd)
				{
					perror("accept");
					close(sfd);
					return -1;
				}	
				printf("newfd is %d\n",newfd);
				printf("connect success,client IP=%s,client port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			}
			if((newfd >0) && FD_ISSET(newfd,&rdset))
			{	
				bzero(buf,sizeof(buf));
				ret=recv(newfd,buf,sizeof(buf),0);	
				if(ret >0)
				{
					printf("%s\n",buf);
				}
			}
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				ret=send(newfd,buf,strlen(buf)-1,0);
				if(-1==ret)
				{
					perror("send");
					close(newfd);
					close(sfd);
					return -1;
				}
			}
		}
	}
	return 0;
}
	

