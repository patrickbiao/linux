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
	shmid=shmget(skey,1<<12,0600|IPC_CREAT);//1<<12表示4K,0600为或上权限，
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	printf("the shmid is %d\n",shmid);	
	char* p;
	p=(char*)shmat(shmid,NULL,0);
	if((char*)-1==p)
	{
		perror("shmat");
		return -1;
	}
	*p='9';
//	while(1);
	return 0;
}
	

