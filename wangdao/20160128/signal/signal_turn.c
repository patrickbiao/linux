#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
void sig(int signum)
{
	printf("the signum is %d\n",signum);
	sleep(3);
	printf("after sleep,the signum is %d\n",signum);
}

int main()
{
	signal(SIGINT,sig);
	while(1);
	return 0;
}

