#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define NUM 10000000
pthread_mutex_t mutex;
int sum=0;
void* p_func(void* p)
{
	int i;
	pthread_mutex_lock(&mutex);
//	printf("I am here\n");
	pthread_mutex_lock(&mutex);//If没有设置属性，则会在这里卡住
	printf("I am child\n");
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main()
{
	int ret;
	pthread_mutexattr_t attr;
	int i;
	i=PTHREAD_MUTEX_RECURSIVE_NP;
	memcpy(&attr,&i,4); 
	ret=pthread_mutex_init(&mutex,&attr);//当加入这个属性之后才可以重复加锁
										 //不然清理函数执行lock就会被挂起
	if(0!=ret)
	{
		printf("pthread_mutex_init ret=%d\n",ret);
		return -1;
	}
	pthread_t pthid;
	pthread_create(&pthid,NULL,p_func,NULL);
	ret=pthread_join(pthid,NULL);
	if(0!=ret)
	{
		printf("pthread_join ret=%d\n",ret);
		return -1;
	}
	ret=pthread_mutex_destroy(&mutex);
	if(0!=ret)
	{
		printf("pthread_mutex_destroy ret=%d\n",ret);
		return -1;
	}
	return 0;
}

