#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

int main()
{
	int fds[2];//放两个描述符，打开文件就获得描述符，这里是内核在pipe中帮忙打开的
	int ret;
	ret =	pipe(fds);//初始化，只要pipe一下，就能读能写
	if (-1==ret)
	{
		perror("pipe:");
		exit(-1);//也可用return -1,因为这里为主函数		
	}
	if(fork()==0)//子进程，把读关掉
	{
		close(fds[0]);//0为标准输入，即进程从标准输入文件（键盘）中读数据
		write(fds[1],"hello",5);//在标准输出中写5个字节的hello
		exit(0);
	}else{//这里不会因为父进程执行比较快，子进程还没来得及写就已经读过了，因为读管道时，如果没内容会阻塞
		close(fds[1]);//子进程关闭写端
		char buf[10];
		bzero(buf,sizeof(buf));
		read(fds[0],buf,sizeof(buf));
		printf("buf is %s\n",buf);
		exit(0);
	}
}	

			
		
