#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
void sig(int signum)
{
	printf("the signum is %d\n",signum);
}

int main()
{
	signal(SIGINT,sig);//将函数名sig直接赋给函数指针，因为函数名就是一个地址
	while(1);
	return 0;
}

