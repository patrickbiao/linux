#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 2
int tickets=10;
pthread_mutex_t mutex;//设置锁为全局变量
pthread_cond_t cond;//设置条件变量为全局变量

void* s_func(void* p)
{
	int i=(int)p;//64位的指针为8个节
	while(1)//不断卖票
	{
		pthread_mutex_lock(&mutex);//先加锁
		if(tickets >0)
		{
			printf("I am windows %d,tickets=%d\n",i,tickets);
			tickets--;
			if(0==tickets)
				pthread_cond_signal(&cond);
			printf("I am windows %d,sale finish,tickets=%d\n",i,tickets);
		}else{
			pthread_mutex_unlock(&mutex);//没票了，解锁
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex);//正常卖，解锁
		sleep(1);
	}
}

void* set_func(void* p)
{
		pthread_mutex_lock(&mutex);//先加锁，因为后面要wait
		if(tickets >0)
			pthread_cond_wait(&cond,&mutex);//wait的条件变量需要与锁结合
		tickets=10;
		pthread_mutex_unlock(&mutex);
	sleep(1);
	pthread_exit(NULL);
}
int main()
{
	pthread_mutex_init(&mutex,NULL);//初始化锁
	pthread_cond_init(&cond,NULL);//初始化条件变量
	pthread_t sale[NUM],setticket;//设置线程ID
	int i;
	for(i=0;i<NUM;i++)
	{
		pthread_create(&sale[i],NULL,s_func,(void*)i);
	}
	pthread_create(&setticket,NULL,set_func,NULL);
	for(i=0;i<NUM;i++)
	{
		pthread_join(sale[i],NULL);//释放资源
	}
	pthread_join(setticket,NULL);
	pthread_mutex_destroy(&mutex);//销毁
	pthread_cond_destroy(&cond);
	return 0;
}

