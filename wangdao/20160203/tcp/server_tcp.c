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
	sfd=socket(AF_INET,SOCK_STREAM,0);//socket初始化一个网络通信的句柄
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in server_addr;//定义结构体变量
	bzero(&server_addr,sizeof(server_addr));//网络结构体都要进行初始化
	server_addr.sin_family=AF_INET;//调用socket（）时的第一个参数，
											 //即AF_INET值代表Ipv4网络协议。
	server_addr.sin_port = htons(atoi(argv[2]));//为使用的port（端口）编号
	//传进来的端口argv[2]是一个字符串,故用atoi转换成整型数
	//htons主机字节序转化成网络字节序
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);//
	//inet_addr()用来将IP地址字符串转换成网络所使用的二进制数字，如果为INADDR_ANY，这表示服务器自动填充本机IP地址。
	int ret;
	ret=bind(sfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));//强转成结构体指针是为了兼容以往的程序	
//用来绑定一个端口号和IP地址，使套接口与指定的端口号和IP地址相关联。
	if(-1==ret)
	{
		perror("bind");
		close(sfd);//因为前面的socket是成功已经生成了
		return -1;
	}
	ret=listen(sfd,NUM);//第二个参数为指定同时能处理的最大连接要求
	if(-1==ret)
	{
		perror("listen");
		close(sfd);
		return -1;
	}
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));
	int length=sizeof(client_addr);
	int newfd;
	char buf[128];
	while(1)
	{
		newfd=accept(sfd,(struct sockaddr*)&client_addr,&length);
		//由于没有select,连接成功执行下面的函数后会处于卡住状态(),
		//等待下一个连接
		if(-1==newfd)
		{
			perror("accept");
			close(sfd);
			return -1;
		}
		printf("connect success,client IP=%s,client port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	//struct in_addr sin_addr;  //为IP 地址
		bzero(buf,sizeof(buf));
		ret=recv(newfd,buf,sizeof(buf),0);	
	//与read的机制很像，返回实际接收到的字符数，只不过这里是从网卡
   //的缓冲区上读数据，由于没有select,没接收到对方的数据会卡住
	//相当于read从管道读数据，如果没有数据会卡住（卡住）
		if(ret >0)
		{
			printf("the message is %s\n",buf);
		}
		bzero(buf,sizeof(buf));//每次都要初始化
		strcpy(buf,"hello");
		ret=send(newfd,buf,strlen(buf),0);
	//send与write很像
		if(-1==ret)
		{
			perror("send");
			close(newfd);
			close(sfd);
			return -1;
		}
	}
	return 0;
}
