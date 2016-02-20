#include <stdio.h>
#include <pthread.h>

void cleanup(void* p)
{
	printf("the p is %d\n",(int)p);//先压栈，后出来，先打印2，再打印1
}

void* pthread_func(void *p)
{
	pthread_cleanup_push(cleanup,(void*)1);
	pthread_cleanup_push(cleanup,(void*)2);
	sleep(2);
	pthread_exit(NULL);
	pthread_cleanup_pop(0);//如果为0,执行到该位置，就是弹出函数不执行清理函数	
	pthread_cleanup_pop(0);	
}

int main()
{
	pthread_t pthid;//定义线程ID
	pthread_create(&pthid,NULL,pthread_func,NULL);//创建子线程
	int ret=pthread_join(pthid,NULL);//不需要接收参数，所以为NULL
												//if要接收参数，其为pthread_exit(p)的
												//返回值取地址&p
	if(0 != ret)
	{
		printf("ret is %d\n",ret);
		return -1;
	}
	return 0;
}

