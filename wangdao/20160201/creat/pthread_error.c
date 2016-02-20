#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t thid[5];//用于放线程ID 
 
void* pthread_entry(void* p)
{
	printf("I am a child\n");
	sleep(2);//主线程也会等待,得放在exit之前
	pthread_exit(NULL);
}


int main()
{
	pthread_t pth_id;//定义线程ID
	int ret;
	pthread_create(&pth_id,NULL,pthread_entry,NULL);//创建子线程，基本不会失败
	ret =pthread_join(0,NULL);//等待子线程退，这里必须写上具体的ID，所以每次只能回收一个线程
	if(0!=ret)
	{
		perror("pthread_join:");//使用pthread系列函数不能使用perror
		printf("ret is %d\n",ret);
		return -1;
	}
//	int i;
//	for(i=0;i<30;i++)
//	{
//		printf("I am main thread\n");
//		sleep(1);
//	}
	return 0;
}

