#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#define PROJ_ID 1
int main(int argc,char* argv[])
{
	if(argc!= 2)
	{
		printf("error args\n");
		return -1;
	}
	key_t skey;
	skey=ftok(argv[1],PROJ_ID);
	if(-1==skey)
	{
		perror("ftok");
		return -1;
	}
	printf("the key is %d\n",skey);
	int shmid;
	shmid=shmget(skey,1<<12,0600|IPC_CREAT);//1<<12表示4K,0600表创建的共享内存可读写，这个要加上
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	printf("the shmid is %d\n",shmid);	
	int* p;
	p=(int*)shmat(shmid,NULL,0);//这是映射后的首地址
	if((int*)-1==p)
	{
		perror("shmat");
		return -1;
	}
	*p=0;//需要给p赋初值,把共享内存的值初始化为0；Linux下的内存得初始化
	int i;
	for(i=0;i<10000000;i++)
	{
		*p = *p+1;
	}
	printf("The p is %d\n",*p);
	return 0;
}
	

