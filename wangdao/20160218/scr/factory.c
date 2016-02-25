#include "factory.h"

void factory_init(pfactory pf,int num,int capacity,pfunc entry)//线程工厂，线程数
{
	int ret;
	ret=pthread_cond_init(&pf->cond,NULL);//初始化条件变量
	if(0!=ret)//判断一下，知道错哪里
	{     
		printf("pthread_cond_init fail\n");//打印错误码
		return;
	}
	pf->pth=(pthread_t*)calloc(num,sizeof(pthread_t));//申请的空间都是用来放pthread id
	pf->entry=entry;
	factory_que_init(&pf->fd_que,capacity);//工厂队列初始化
	pf->start_flag=0;//0没启动
	pf->thread_num=num;	
}//赋值完毕，可以start
                        
void factory_start(pfactory pf)//循环创建子线程
{
	if(0 == pf->start_flag)//为0才创建，不然是创建过的，避免重复启动
	{
		int i;
		int ret;
		for(i=0;i<pf->thread_num;i++)
		{
			ret=pthread_create(&pf->pth[i],NULL,pf->entry,(void*)pf);//最后一个参数为线程传参，
			//这样每个线程都拿到全局的pf结构体指针，这个工厂结构体pf里边什么东西都有
			if(0!=ret)
			{
				printf("pthread_create fail\n");
				return;
			}
		}
		pf->start_flag=1;//启动完毕，将值设为1，不然会重新进去创建
	}
}

