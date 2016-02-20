#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
void* thread_func(void* p)
{
	printf("I am child\n");
	pthread_exit(NULL);
}

int main()
{
	pthread_t thid;
	pthread_create(&thid,NULL,thread_func,NULL);//创建子线程
	int ret=pthread_join(thid,NULL);//等待子线程退出
	if(0 !=ret)
	{
		perror("pthread_join");
		printf("ret is %d\n",ret);
		return -1;
	}
	return 0;
}

