#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	pid_t pid;
	pid=fork();
	if(pid ==0)
	{
		printf("I am child\n");
//		exit(1);
	}else{
		int status;
		int ret;
		sleep(1);
		ret=waitpid(pid,&status,WNOHANG);
		printf("ret is %d\n",ret);//查看WNOHANG情况下的返回值
		if(ret < 0)
		{
			perror("waitpid");
			return -1;
		}
		if(WIFEXITED(status))
		{
			printf("the exit value is %d\n",WEXITSTATUS(status));
		}
		return 0;
	}
}

