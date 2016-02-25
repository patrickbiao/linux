#ifndef __FACTORY__//加上这个之后就不担心不同头文件是否定义了相同数据结构
#define __FACTORY__
#include "head.h"
#include "work_que.h"

#define DOWN_FILE "hello.avi"
typedef void* (*pfunc)(void*);//后面用时，可以直接写类型，不用带参数 
typedef struct thread_pool{//线程池
	pthread_cond_t cond;
	pthread_t* pth;//定义成指针是不知道这里有多少个线程，即pth这个入参是动态的，所以需要malloc
	pfunc entry;
	que_t fd_que;//这里的队列为什么不定义成一个指针，因为这里只有一个队列,个数已经确定了，
				 //所以使用栈空间就行（因为这里所需空间较小,几十个字节），不需要堆空间（栈比堆快）
	int start_flag;//线程是否启动标志
	int thread_num;//线程个数
}factory,*pfactory;

typedef struct tdata{
	int len;
	char buf[1000];
}data_t,*pdata_t;

void factory_init(pfactory,int,int,pfunc);//线程结构体指针，num,capacity，
//函数指针（上面定义了，这里直接写类型，也可以是pfunc p,定义pfunc 类型的p函数）
void factory_start(pfactory);//工厂启动，创建线程，所以传指针，
//该工厂已经将线程数传进去了，后面的很多函数也是直接传工厂就行，因为工厂已经全部准备好了 
void* thread_handle(void*);
void send_file(int);
void send_n(int,char*,int);
#endif

