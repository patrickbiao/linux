#include "head.h"

void make_child(pchild cptr,int n)
{
	int i;
	int fds[2];
	int ret;
	int pid;
	for(i=0;i<n;i++)
	{
		ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);	
		if(-1==ret)
		{
			perror("socketpair");
			return;
		}
		pid=fork();
		if(pid==0)
		{
			child_handle();
		}
		cptr[i].pid=pid;
		cptr[i].fds[0]=fds[0];
		cptr[i].fds[1]=fds[1];
		cptr[i].busy=0;
	}
}

void child_handle()
{
	while(1);
}

