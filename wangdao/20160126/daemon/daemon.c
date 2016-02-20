#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
//1创建子进程:让父进程退出，子进程接着往下执行
	if(fork()>0)
		exit(0);
//2在子进程中创建新会话
	setsid();//把自己变成会话组组长（老大）
//3改变当前目录为根目录
	chdir("/");
//4重设文件掩码，将掩码变为0
	umask(0);
//5关闭所有不需要的文件描述符
 	int i;
	for(i=0;i<3;i++)	
		close(i);
	while(1);//为了看其状态
	return 0;
}
