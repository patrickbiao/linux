#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <stdlib.h>
void siga(int signum,siginfo_t *p,void* p1)//信号处理函数
{
	printf("I get the sig %d\n",signum);
	printf("the send process id is %d\n",p->si_pid);//siginfo_t *p这个函数是内核调用的
	printf("the send uid is %d\n",p->si_uid);//真实用户ID
}

int main()
{
	struct sigaction act;
	bzero(&act,sizeof(act));
	act.sa_sigaction = siga;
	sigset_t sa_mask;
	sigemptyset(&act.sa_mask);
	//memcpy(&act.sa_mask,&sa_mask,sizeof(sa_mask));
	act.sa_flags=SA_SIGINFO;	
	int ret;
	ret=sigaction(SIGINT,&act,NULL);//设置信号处理行为
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	while(1);
	return 0;
}
	



