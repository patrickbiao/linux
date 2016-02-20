#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_cond_t cond;//定义一个条件变量
pthread_mutex_t mutex;//定义锁

void* p_func(void* p)
{
	pthread_mutex_lock(&mutex);//wait之前在当前线程加锁
	int ret;
	printf("I am child,I am here\n");
	ret=pthread_cond_wait(&cond,&mutex);//等待时不会返回，
													//if没有signal会一直wait
													//即一直卡在这里
	if(0!=ret)
	{
		printf("pthread_cond_wait ret=%d\n",ret);
	}
	printf("I am child thread,I am wake\n");
	pthread_mutex_unlock(&mutex);//保持对称
	pthread_exit(NULL);
}
int main()
{
	int ret;
	ret=pthread_cond_init(&cond,NULL);//条件变量初始化
	if(0!=ret)
	{
		printf("pthread_cond_init ret=%d\n",ret);
		return -1;
	}
	ret=pthread_mutex_init(&mutex,NULL);//必须是快速锁（NUL），才能进行wait
	if(0!=ret)
	{
		printf("pthread_mutex_init ret=%d\n",ret);
		return -1;
	}
	pthread_t thid;
	pthread_create(&thid,NULL,p_func,NULL);
	sleep(1);
	pthread_mutex_lock(&mutex);//验证子线程wait后，是否解锁，睡觉
										//当一个线程上锁时，其余请求锁的线程阻塞
	printf("I am main thread,I can lock\n");
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	ret=pthread_join(thid,NULL);
	if(0!=ret)
	{
		printf("pthread_join ret=%d\n",ret);
		return -1;
	}
	ret=pthread_cond_destroy(&cond);//销毁条件变量
	if(0!=ret)
	{
		printf("pthread_cond_destroy ret=%d\n",ret);
		return -1;
	}
	pthread_mutex_destroy(&mutex);//销毁锁
	return 0;
}

