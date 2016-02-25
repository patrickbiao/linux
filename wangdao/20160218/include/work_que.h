#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"

typedef struct node_fd{
	int new_fd;
	struct node_fd* pnext;
}node,*pnode;

typedef struct que_fd{//描述符队列，也称工作队列
	pnode que_head,que_tail;//队列头尾
	pthread_mutex_t mutex;//队列的锁
	int size;//当前放置的描述符个数
	int que_capacity;//队列最大长度，通过传参传进来
}que_t,*pque_t;

void factory_que_init(pque_t,int);//声明只需写类型就行 
void factory_que_set(pque_t,pnode);//往链表中增加元素，
void factory_que_get(pque_t,pnode*);//从链表中获取元素，并删除;对调用函数中的指针值改变，要传二级指针
int factory_que_empty(pque_t);
#endif

