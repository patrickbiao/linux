#include <stdio.h>
#include <pthread.h>

#define NUM 10000000
pthread_mutex_t mutex;//定义锁
int sum=0; 
void* p_func(void* p)
{
	int i;
	for(i=0;i<NUM;i++)
	{
		pthread_mutex_lock(&mutex);//锁要加在循环中
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	printf("child sum is %d\n",sum);
	pthread_exit(NULL);
}
int main()
{
	int ret;
	ret=pthread_mutex_init(&mutex,NULL);//对锁进行初始化，没加上得不到正确结果	
	if(0!=ret)
	{
		printf("pthread_mutex_init ret=%d\n",ret);
		return -1;
	}
	pthread_t pthid;//定义子线程ID
	pthread_create(&pthid,NULL,p_func,NULL);//创建子线程
	int i;
	for(i=0;i<NUM;i++)
	{
		pthread_mutex_lock(&mutex); 
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	printf("the sum is %d\n",sum);		
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

