#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid,ppid;
	pid = fork();
	if(pid==0){
		printf("my pid is %d,ppid is%d\n",getpid(),getppid());
		exit(0);
	}else
		{
			printf("my pid is %d,ppid is%d\n",getpid(),getppid());
			while(1);
		}
 	return 0;
}
