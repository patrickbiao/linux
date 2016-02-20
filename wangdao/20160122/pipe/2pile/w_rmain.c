#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
		  if (argc != 3){
					 printf("error args\n");
					 return -1;	
		  }
		  int fdw;
		  fdw = open(argv[1],O_WRONLY);
		  if (-1 == fdw){
					 perror("open1:");
					 return -1;
		  }
		  //	printf("the fdw is %d\n",fdw);
		  //		while(1);
		  int fdr;
		  fdr = open(argv[2],O_RDONLY);
		  if (-1 == fdr){
					 perror("open2:");
					 return -1;
		  }
		  int ret;
		  printf("fdr = %d, fdw = %d\n",fdr,fdw);
		  char buf[128];
		  while(bzero(buf,sizeof(buf)),(ret=read(0,buf,sizeof(buf)))>0){
					// printf("%s\n",buf);
		  write(fdw,buf,ret-1);
bzero(buf,sizeof(buf));
		  if((read(fdr,buf,sizeof(buf))>0)){
					 printf("%s\n",buf);
					 if(strcmp(buf,"Bye Bye")==0)
								break;
		  		}
			}
close(fdr);
close(fdw);
		  return 0;
}

