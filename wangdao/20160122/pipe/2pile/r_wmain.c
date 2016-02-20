#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
int main(int argc,char* argv[])
{
		  if (argc != 3){
					 printf("error args\n");
					 return -1;	
		  }
		  int fdr;
		  fdr = open(argv[1],O_RDONLY);
		  if (-1 == fdr){
					 perror("open1:");
					 return -1;
		  }
		  int fdw;
		  fdw = open(argv[2],O_WRONLY);
		  if (-1 == fdw){
					 perror("open2:");
					 return -1;
		  }
		  int ret;
		  char buf[128];
		  printf("fdr = %d ,fdw = %d\n",fdr,fdw);

		  while(bzero(buf,sizeof(buf)),read(fdr,buf,sizeof(buf))>0)
		  {
					 //	printf("the ret is %d\n",ret);
					 printf("%s\n",buf);
					 bzero(buf,sizeof(buf));
					 if((ret=read(0,buf,sizeof(buf)))>0){
								write(fdw,buf,ret-1);
					 }else{
								write(fdw,"Bye Bye",7);
								break;
					 }
		  }
		  return 0;
}

