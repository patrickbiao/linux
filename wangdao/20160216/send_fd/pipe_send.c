#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	int fds[2];//定义一个数组来存放管道描述符fds[0]读，fds[1]写
	pipe(fds);//初始化无名管道,半双工，固定读写端
	if(!fork())//子进程，fork之后，子进程返回0
	{
		close(fds[1]);//关闭管道的写端
		int fd;
		read(fds[0],&fd,4);//子进程从fds[0]中读fd
		printf("i am child,fd is %d\n",fd);
		char buf[10];
		int ret;
		ret=read(fd,buf,sizeof(buf));//从fd中读数据
		if(-1==ret)
		{
			perror("read");
			return -1;
		}
		exit(0);
	}else{
		close(fds[0]);//关闭管道读端
		int fd = open(argv[1],O_RDWR);//父进程打开文件,返回文件描述符
		if(-1==fd)
		{
			perror("open");
			return -1;
		}
		printf("i am parent,fd is %d\n",fd);
		write(fds[1],&fd,4);//往fds[1]写，将fd写到管道
		wait(NULL);//等待子进程结束
		exit(0);
	}
}
