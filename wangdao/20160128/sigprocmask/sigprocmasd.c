#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	sigset_t set;//定义一个这样的集合
	sigemptyset(&set);//将集合清空（取地址）
	int ret;
	ret=sigaddset(&set,SIGINT);//在集合中添加了2号信号
	if(-1==ret)
	{
		perror("sigaddset");
		return -1;
	}
	ret=sigprocmask(SIG_BLOCK,&set,NULL);//全程阻塞2号信号
	if(-1==ret)
	{
		perror("sigprocmask");
		return -1;
	}
	sleep(5);//等待输入信号，不然直接卡在while中，该函数也是被信号焕醒的，一旦sleep,系统设置了一个tamer,
			  //这个tamer过10s钟之后会发一个alarm信号将其焕醒
			  //同时，在sleep的一瞬间，它就通过系统的专用接口把cpu放弃了
			  //这时CPU就还给调度器
	sigemptyset(&set);//清空该集合，该集合是我们自己定义的，
					  //我们只能操作阻塞集合，内核中的信号集合操作不了
	sigpending(&set); //这时会将挂起的信号放到里边
	if(1==sigismember(&set,SIGINT))//查看2号信号是否在挂起
	{
		printf("the SIGINT is pending\n");
	}	
	while(1);
	return 0;
}

