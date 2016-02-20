#include "func.h"

//初始化sokect
int main(int argc,char **argv)//因为需要传ip地址和端口号
{
	/*1.初始化socket*/
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	/*2.绑定对应的端口号和IP地址到sfd上*/
	struct sockaddr_in server;//定义一个存放信息的结构体
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;//ipv4oripv6标识
	//该结构体是进行网络传输时的数据（而上面是本地标识的类型），这里需要重新写一下标识，与对端进行通信时才知道是ipv4
	server.sin_port=htons(atoi(argv[2]));//字符串转整形
	server.sin_addr.s_addr=inet_addr(argv[1]);//传进来的点分十进制转成32位二进制
	int ret;
	ret = bind(sfd,(struct sockaddr *)&server,sizeof(struct sockaddr));
	if (-1==ret)
	{
		perror("bind");
		return -1;
	}
	printf("bind ok\n");
	char buf[128];
	while(1)
	{
	bzero(buf,sizeof(buf));
	struct sockaddr_in client;//用于存放客户端信息
	bzero(&client,sizeof(client));
	int clen=sizeof(struct sockaddr);
	ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&client,&clen);//recv时，拿到客户端的IP地址和端口号（系统自动分配的）
	if(-1==ret)
	{	
		perror("recvfrom");
		return -1;
	}
 	//printf("i am server i recvfrom %s\n",buf);
	//printf("IP=%s,PORT=%d",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	printf("%s\n",buf);
	bzero(buf,sizeof(buf));
	read(0,buf,sizeof(buf));
	ret=sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr *)&client,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("sendto");
		return -1;
	}
  	}
	close(sfd);
	return 0;
}
