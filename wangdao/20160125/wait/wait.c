#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	if(!fork())
	{
		printf("I am child\n");
		exit(5);
	}else{
		int status;
		int ret;
		ret=wait(&status);
		if(ret < 0)
		{
			perror("wait");
			return -1;
		}
		if(WIFEXITED(status))
		{
			printf("the status is %d\n",status);		
			printf("the exit value is %d\n",WEXITSTATUS(status));
		}
		return 0;
	}
}

