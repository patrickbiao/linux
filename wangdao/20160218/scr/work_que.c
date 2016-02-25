#include "work_que.h"

void factory_que_init(pque_t pq,int capacity)//队列初始化
{
	int ret;
	pq->que_head=NULL;//一开始队列什么都没有，所以为null
	pq->que_tail=NULL;
	ret=pthread_mutex_init(&pq->mutex,NULL);
	if(0!=ret)//判断锁是否异常
	{
		printf("pthread_mutex_init fail\n");
		return;
	}
	pq->size=0;//当前放置描述符个数，还没start，为0
	pq->que_capacity=capacity;//队列长度
}

void factory_que_set(pque_t pq,pnode pnew)//增加描述符用尾插法
{
	pthread_mutex_lock(&pq->mutex);//在操作队列（链表）之前要加锁，锁在pque_t已经定义好
	if(pq->que_tail==NULL)//判断队列是否为空（即没有节点），可以通过判断头or尾节点是否为空
	{
		pq->que_head=pnew;
		pq->que_tail=pnew;
	}else{
		(pq->que_tail)->pnext=pnew;//que_tail本身是一个pnode类型指针，pnode中pnext是节点结构体（包含描述符和next）
		pq->que_tail=pnew;
	}
	pq->size++;//插完队列中描述符的个数增加1
	pthread_mutex_unlock(&pq->mutex);//插好之后解锁
}

void factory_que_get(pque_t pq,pnode* pcur)//获取当前节点，并从头部删除
{
	pthread_mutex_lock(&pq->mutex);//获取前加锁
	if(pq->que_head !=NULL)//也可以不用判断，因为main.c中已经先判断了
	{
		*pcur=pq->que_head;//pcur为二级指针，将头节点赋给它
		pq->que_head=(*pcur)->pnext;	
		pq->size--;
	}
	pthread_mutex_unlock(&pq->mutex);
}

int factory_que_empty(pque_t pq)
{
	pthread_mutex_lock(&pq->mutex);//先加锁
	if(pq->size == 0)
	{
		return 1;
	}
	pthread_mutex_unlock(&pq->mutex);
	return 0;
}

