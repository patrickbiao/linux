#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <strings.h>
#define TIMES 10000000
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
	shmid=shmget(skey,1<<12,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	printf("the shmid is %d\n",shmid);	
	int* p;
	p=(int*)shmat(shmid,NULL,0);
	if((int*)-1==p)
	{
		perror("shmat");
		return -1;
	}
	int semid,ret;
	semid=semget((key_t)1234,1,0600|IPC_CREAT);
	ret=semctl(semid,0,GETVAL);
	if(-1==ret)
	{
		perror("semctl2");
		return -1;
	}
	printf("The default semaphore is %d\n",ret);
	ret=semctl(semid,0,SETVAL,1);
	if(-1==ret)
	{
		perror("semctl1");
		return -1;
	}
	printf("after set,ret is %d\n",ret);
	if(-1==semid)
	{
		perror("semget");
		return -1;
	}
	struct sembuf sop;
	bzero(&sop,sizeof(sop));
	sop.sem_num=0;
	sop.sem_op=-1;
	sop.sem_flg=SEM_UNDO;	
	
	struct sembuf sov;
	bzero(&sov,sizeof(sov));
	sov.sem_num=0;
	sov.sem_op=1;
	sov.sem_flg=SEM_UNDO;	
	int i;
	for(i=0;i<TIMES;i++)
	{
		semop(semid,&sop,1);
		*p=(*p)+1;
		semop(semid,&sov,1);
	}
	printf("the value is %d\n",*p);
	return 0;
}
	

