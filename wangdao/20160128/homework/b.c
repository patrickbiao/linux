#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int fdr,fdw,shmid;
char *p;
void sig(int signum)
{
	int ret;
	close(fdr);
	close(fdw);
	shmdt(p);
	ret=shmctl(shmid,IPC_RMID,NULL);
	if(ret==-1)
	{
		perror("shmctl:");
		return;
	}else{
	printf("Bye bye\n");
	exit(0);
	}
}
int main(int argc,char* argv[])
{
	signal(SIGINT,sig);

   /*创建共享内存*/
//	int shmid;
	shmid = shmget((key_t)2222,4096,0600|IPC_CREAT);
   if(-1==shmid)
   {
      perror("shmget:");
      return -1;
   }
   printf("shmid is %d\n",shmid);
   char *p;
   p = (char*)shmat(shmid,NULL,0);
   if((char*)-1==p)
   {
      perror("shmat:");
      return -1;
   }
   bzero(p,4096);


    
    /*创建信号量*/
    int semid,ret;
    semid=semget((key_t)2222,1,0600|IPC_CREAT);
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
    printf("semid is %d\n",semid);
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
	if(argc!=3){
		printf("error args\n");
	}
	fdw = open(argv[1],O_WRONLY);
	if(-1==fdw)
	{
		perror("open1:");
	}
	fdr = open(argv[2],O_RDONLY);
	if(-1==fdr)
    {
		 perror("open1:");
	 }
	 printf("fdr=%d,fdw=%d\n",fdr,fdw);
	 printf("successfully connected\n");
	fd_set readset;
//	int ret;
	char buf[128];
  time_t now;
  struct tm *tm_now;
  char *datetime;
  bzero(buf,sizeof(buf));
  time(&now);
  tm_now = localtime(&now);
	while(2){
       FD_ZERO(&readset);
       FD_SET(0,&readset);
       FD_SET(fdr,&readset);
	 	 ret = select(fdr+1,&readset,NULL,NULL,NULL);
			if (ret>0){
				if(FD_ISSET(0,&readset)){
            datetime = asctime(tm_now);

				if((ret=read(0,buf,sizeof(buf)))>0){
						write(fdw,buf,ret-1);
						buf[ret-1] = 0;
						if(strcmp(buf,"bye")==0)
						strcpy(p,buf);
//						semop(semid,&sop,1);
//						sprintf(p,"%s%s%s","[me]",datetime,buf);
//						semop(semid,&sov,1);
				}else{
								write(fdw,"bye",3);
										break;}}
					if(FD_ISSET(fdr,&readset))
					{
							bzero(buf,sizeof(buf));
							if(read(fdr,buf,sizeof(buf))>0){
						//	printf("%s\n",buf);
							semop(semid,&sop,1);
						if(strcmp(buf,"bye")==0)
						{
							strcpy(p,buf);
						}else
						 sprintf(p,"%s%s%s","[from a]",datetime,buf);
					  //	strcpy(p,buf);
//							printf("%s\n",p);
							semop(semid,&sov,1);
							}
						//	if(strcmp(buf,"bye")==0)
						//		break;
					}
}
}
close(fdr);
close(fdw);
return 0;
}
					                        
							
		

