#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_t thid[5];//用于放线程ID 
char* p;//全局变量默认值为0 
void* pthread_entry(void* p1)
{
	printf("I am a child\n");
   p =(char*)malloc(10);//正常情况下需要在子线程中free
//	sleep(2);//主线程也会等待,得放在exit之前
	free(p);
	p =NULL;
	pthread_exit(NULL);
}


int main()
{
	pthread_t pth_id;//定义线程ID
	int ret;
	pthread_create(&pth_id,NULL,pthread_entry,NULL);//创建子线程，基本不会失败
	ret =pthread_join(pth_id,NULL);//等待子线程退出，当子进程没退出时在这里卡住
                                  //这里必须写上具体的ID，
											 //所以每次只能回收一个线程
	if(0!=ret)
	{
		printf("ret is %d\n",ret);
		return -1;
	}
	printf("the p is %x\n",p);//打印p的地址用16进制
	strcpy(p,"hello");
	printf("the p is %s\n",p);
	return 0;
} 

