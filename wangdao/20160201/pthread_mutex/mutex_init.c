#include <stdio.h>
#include <pthread.h>

int main()
{
	pthread_mutex_t mutex;
	int ret;
	ret=pthread_mutex_init(&mutex,NULL);	
	if(0!=ret)
	{
		printf("pthread_mutex_init ret=%d\n",ret);
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

