 #include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#define PROJ_ID 1
int main(int argc,char* argv[])
{
	int shmid;
	shmid=shmget((key_t)1234,1<<12,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	printf("the shmid is %d\n",shmid);	
	return 0;
}
	

