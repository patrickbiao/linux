#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc,char* argv[])//传参
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}
	struct in_addr addr;//用于存放32位二进制的地址
	int ret;
	ret=inet_aton(argv[1],&addr);//将点分十进制转换为网络32位二进制
	if(1 != ret)
	{
		perror("inet_aton");
		return -1;
	}
	printf("the s_addr is %x\n",addr.s_addr);
	printf("string is %s\n",inet_ntoa(addr));
	printf("the s_addr 2 is %x\n",inet_addr(argv[1]));
	return 0;
}

