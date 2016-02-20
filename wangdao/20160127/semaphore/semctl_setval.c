#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int semid;
	semid=semget((key_t)1234,1,0600|IPC_CREAT);
	if(-1==semid)
	{
		perror("semget");
		return -1;
	}	
	int ret;
	ret=semctl(semid,0,GETVAL);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	printf("ret is %d\n",ret);
	ret=semctl(semid,0,SETVAL,1);
	if(-1==ret)
	{
		perror("semctl1");
		return -1;
	}
	ret=semctl(semid,0,GETVAL);
	if(-1==ret)
	{
		perror("semctl2");
		return -1;
	}
	printf("after set,ret is %d\n",ret);
	return 0;
}
