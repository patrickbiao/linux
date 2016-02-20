#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
void siga(int signum,siginfo_t *p,void* p1)//信号处理函数
{
	printf("I get the sig %d\n",signum);
}

int main()
{
	struct sigaction act;
	bzero(&act,sizeof(act));
	act.sa_sigaction = siga;
	sigset_t sa_mask;
	sigemptyset(&act.sa_mask);
	//memcpy(&act.sa_mask,&sa_mask,sizeof(sa_mask));
	act.sa_flags=SA_SIGINFO|SA_RESTART;	
	int ret;
	ret=sigaction(SIGINT,&act,NULL);//设置信号处理行为
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	char buf[10]={0};
	ret=read(0,buf,sizeof(buf));//在read进行阻塞
	printf("buf is %s\n",buf);//if没有或上SA_RESTART,则buf是空的
                             //因为收到中断信号后直接退出阻塞
									  //即退出read
	return 0;
} 
	



