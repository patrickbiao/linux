#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 5
pthread_cond_t cond;//这里的多线程用的条件变量
pthread_mutex_t mutex;//和mutex都为同一个

void* p_func(void* p)
{
	pthread_mutex_lock(&mutex);
	int ret;
	int i=(int)p;
	printf("I am child %d,I am here\n",i);
	ret=pthread_cond_wait(&cond,&mutex);
	if(0!=ret)
	{
		printf("pthread_cond_wait ret=%d\n",ret);
	}
	printf("I am child thread %d,I am wake\n",i);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main()
{
	int ret;
	ret=pthread_cond_init(&cond,NULL);//初始化条件变量
	if(0!=ret)
	{
		printf("pthread_cond_init ret=%d\n",ret);
		return -1;
	}
	ret=pthread_mutex_init(&mutex,NULL);//初始化锁
	if(0!=ret)
	{
		printf("pthread_mutex_init ret=%d\n",ret);
		return -1;
	}
	pthread_t thid[NUM];//定义多个子线程ID
	int i; 
	for(i=0;i<NUM;i++)//for循环创建多个线程
	{
		pthread_create(&thid[i],NULL,p_func,(void*)i);//创建多线程时，子线程的函数可以不变，但前
													  //面的ID就被后面的ID覆盖了，所以要定义数组
	}
	sleep(1);
//	pthread_cond_broadcast(&cond);
	pthread_cond_signal(&cond);
	for(i=0;i<NUM;i++)//相对应for循环回收资源
	{
		ret=pthread_join(thid[i],NULL);
		if(0!=ret)
		{
			printf("pthread_join ret=%d\n",ret);
			return -1;
		}
	}
	ret=pthread_cond_destroy(&cond);
	if(0!=ret)
	{
		printf("pthread_cond_destroy ret=%d\n",ret);
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}

