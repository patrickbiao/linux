#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <stdlib.h>
void siga(int signum,siginfo_t *p,void* p1)//信号处理函数
{
	printf("I get the sig %d\n",signum);
}

int main()
{
	struct sigaction act; 
	bzero(&act,sizeof(act));//将结构体初始化为空
	act.sa_sigaction = siga;
	sigset_t sa_mask;//定义一个掩码
	sigemptyset(&act.sa_mask);//将sa_mask这个集合弄空 
	//memcpy(&act.sa_mask,&sa_mask,sizeof(sa_mask));//将sa_mask拷贝到act_samask
	act.sa_flags=SA_SIGINFO;//这个必须加上   	
	int ret;
	ret=sigaction(SIGINT,&act,NULL);//用该函数进行注册，设置信号处理行为
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	while(1);//让其有时间接收信号
	return 0;
}
	



