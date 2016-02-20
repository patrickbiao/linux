#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int semid;
	semid=semget((key_t)12,10,0600|IPC_CREAT);
	if(-1==semid)
	{ 
		perror("semget");
		return -1;
	}
	return 0;
}


