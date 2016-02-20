#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	typedef void (*sigp)(int);
	sigp p;
	p=signal(SIGINT,SIG_IGN);
	if(SIG_ERR==p)
	{
		perror("signal");
		return -1;
	}
//	while(1);
	printf("i am here\n");
//	kill(getpid(),SIGQUIT);
	sleep(10);
	return 0;
}

