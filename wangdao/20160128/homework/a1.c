#include <signal.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

char *p;
int shmid;
int fdr,fdw;
void sig(int shmid)
{
	int ret;
   close(fdr);
   close(fdw);
  // shmdt(p);
  	ret =  shmctl(shmid,IPC_RMID,NULL);
	if(-1==ret)
	{
		perror("shmctl:");
		return;
	}else{
   printf("Bye by\n");
   exit(0) ;
}
}

int main(int argc,char* argv[])
{
   
//	signal(SIGINT,sig);
   /*创建共享内存*/
   shmid = shmget((key_t)1111,4096,0600|IPC_CREAT);
   if(-1==shmid)
   {
      perror("shmget:");
      return -1;
   }
   printf("shmid is %d\n",shmid);
   p = (char*)shmat(shmid,NULL,0);
   if((char*)-1==p)
   {
      perror("shmat:");
      return -1;
   }
// 	bzero(p,4096); 
	int ret,semid;
	semid=semget((key_t)1111,1,0600|IPC_CREAT);//创建信号量
	ret=semctl(semid,0,GETVAL);
	if(-1==ret)
	{
		perror("semctl2");
		return -1;
	}
	printf("semid is %-d\n",semid);
	printf("The default semaphore is %d\n",ret);
//	ret=semctl(semid,0,SETVAL,1);//只能在一个函数中设置信号量的值，否则相当于将信号量的值设置为2
//	if(-1==ret)
//	{
//		perror("semctl1");
//		return -1;
//	}
//	printf("after set,ret is %d\n",ret);	
	if(-1==semid)
	{
		perror("semget");
		return -1;
	}
	struct sembuf sop;
	bzero(&sop,sizeof(sop));//该函数的第一个元素为指针
	sop.sem_num=0;
	sop.sem_op=-1;
	sop.sem_flg=SEM_UNDO;	
	
	struct sembuf sov;//需要两个结构体数组，一个进行P操作，另一个进行V操作
	bzero(&sov,sizeof(sov));
	sov.sem_num=0;//代表第一个信号量的编号为0
	sov.sem_op=1;
	sov.sem_flg=SEM_UNDO;	
	while(1)
	{
		ret = semop(semid,&sop,1);//P操作,将得到信号量，并可以进入临界区域
		if((*p!=0)&&(ret==0))
		{		
			printf("%s\n",p);
//			printf("%s\n",time());
			if(strcmp(p,"bye")==0)
				break;
			bzero(p,4096);
		}
//		printf("p is %s\n",p);
//		printf("ret is %d\n",ret);
		semop(semid,&sov,1);//V操作,发送信号通知信号量现在可用
	//	printf("I am here\n");
	}
	return 0;
}
