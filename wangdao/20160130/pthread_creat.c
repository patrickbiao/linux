#include <stdio.h>
#include <pthread.h>

void* pthread_entry(void* p)//这里的函数要与函数指针对应，要传参
{
	printf("thread create success\n");
	pthread_exit(NULL);//必须加上，不然会出现标准输出缓冲区异常，打印不出来
} 


int main()
{
	pthread_t pth_id;//定义线程ID
	int ret;
	ret=pthread_create(&pth_id,NULL,pthread_entry,NULL);
	if(-1==ret)
	{
		perror("pthread_create");
		printf("ret is %d\n",ret);//返回的是一个error number
		return -1;
	}
	while(1);
	return 0;
}

