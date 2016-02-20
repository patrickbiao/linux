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
	printf("connect success\n");
	int len;
	data_t buf;
	bzero(&buf,sizeof(buf));
	ret=recv(sfd,&buf.len,4,0);//从sfd中接收4个字节到len中
	if(-1==ret)
	{
		perror("recv1");
		return -1;
	}
	recv(sfd,buf.buf,buf.len,0);//从sfd中接收len个字节到buf,将文件名读进来
	int fd;
	fd=open(buf.buf,O_RDWR|O_CREAT,0666);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	int flag=0;
	while(1)//循环地写
	{	
	    bzero(&buf,sizeof(buf));
		ret=recv(sfd,&buf.len,4,0);//先接四个字节（len,里边的内容是buf的长度），再接后面的
		if(-1==ret)
		{
			perror("recv2");
			return -1;
		}
		ret=recv(sfd,buf.buf,buf.len,0);//从sfd中读len长度到buf中
		if(-1==ret)
		{
			perror("recv3");
			return -1;
		}
		if(buf.len==4 && !memcmp(buf.buf,&flag,4))//当len长度为4时且buf为0时，说明没有了
		{
			break;//接收完
		}
		ret=write(fd,buf.buf,buf.len);//将buf写len个字节到文件fd中去
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}
	close(sfd);
	return 0;
}

