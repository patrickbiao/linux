#include <time.h>
#include <signal.h>
#include <sys/types.h>
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
#include <string.h>

char *p;
int shmid;
int fdr,fdw;
void sig(int s)
{
	int ret;
	close(fdr);
	close(fdw);
	if(shmdt(p)== (void*)-1)
	{
		perror("shmdt");
	}
	ret = shmctl(shmid,IPC_RMID,NULL);
	if(ret==-1)
	{
		perror("shmctl:");
		return;
	}else{
	printf("\nBye bye\n");
	exit(0) ;
	}
}

int main(int argc,char* argv[])
{
 	
	signal(SIGINT,sig);
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
	bzero(p,4096);

	/*创建信号量*/ 
	int semid,ret;
	semid=semget((key_t)1111,1,0600|IPC_CREAT);
	ret=semctl(semid,0,GETVAL);
	if(-1==ret)
	{
		perror("semctl2");
		return -1;
	}
	ret=semctl(semid,0,SETVAL,1);//只能在一个函数中设置信号量的值，否则相当于将信号量的值设置为2
	if(-1==ret)
	{
		perror("semctl1");
		return -1;
	}
	if(-1==semid)
	{
		perror("semget");
		return -1;
	}
	
	struct sembuf sop;
	bzero(&sop,sizeof(sop));//该函数的每一个元素为指针
	sop.sem_num=0;
	sop.sem_op=-1;
   sop.sem_flg=SEM_UNDO;	
	struct sembuf sov;//需要两个结构体数组，一个进行P操作，另一个进行V操作
	bzero(&sov,sizeof(sov));
	sov.sem_num=0;//代表第一个信号量的编号为0
	sov.sem_op=1;
	
/*创建两条通信管道*/

	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	fdr = open(argv[1],O_RDONLY);
	if (fdr==-1)
	{
		perror("open1:");
		return -1;
	}
	fdw = open(argv[2],O_WRONLY);
	if(-1==fdw)
	{
		perror("open2");
		return -1;
	}
	printf("semid is %d\n",semid);
	printf("fdr=%d,fdw=%d\n",fdr,fdw);
	printf("successful connected\n");
	char buf[128];
	fd_set readset;
	 
	time_t now;
    struct tm *tm_now;
    char *datetime;
 
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(0,&readset);
		FD_SET(fdr,&readset);
		ret = select(fdr+1,&readset,NULL,NULL,NULL);
	   time(&now);
   	tm_now = localtime(&now);
  		datetime = asctime(tm_now);
		if(ret>0){
		   if(FD_ISSET(0,&readset)){
				bzero(buf,sizeof(buf));
				if((ret=read(0,buf,sizeof(buf)))>0){
//					printf("ret is %d\n",ret);
					write(fdw,buf,ret-1);
					buf[ret-1]=0;
//					semop(semid,&sop,1);
//					sprintf(p,"%s%s%s","[me]",datetime,buf);
				 	if(strcmp(buf,"bye")==0)
					strcpy(p,buf);
//					printf("ret is %d\n",ret);
//				   semop(semid,&sov,1);
				}else{write(fdw,"bye",3);
						break;}}
		   	if(FD_ISSET(fdr,&readset)){
					bzero(buf,sizeof(buf));
					if((ret=read(fdr,buf,sizeof(buf))>0)){
						//buf[ret-1] = 0;
						semop(semid,&sop,1);//P操作,将得到信号量，并可以进入临界区域
				   	//strcpy(p,buf);
						if(strcmp(buf,"bye")==0)
						{
							strcpy(p,buf);
						}else
						sprintf(p,"%s%s%s","[from b]",datetime,buf);
					//	printf("%s\n",datetime);
					//	ret=semctl(semid,0,SETVAL,1);//只能在一个函数中设置信号量的值，否则相当于将信号量的值设置为2
						semop(semid,&sov,1);//V操作,发送信号通知信号量现在可用	sov.sem_flg=SEM_UNDO;	
					//	printf("%s\n",p);
						
		//			if(strcmp(buf,"bye")==0)
		//				break;
					}
				}
			
		}
}
}
