#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>

int main(int argc,char* argv[])
{
		  if (argc != 2){
					 printf("error args\n");
					 return -1;	
		  }
		  int fdr;
		  fdr = open(argv[1],O_RDONLY);
		  if (-1 == fdr){
					 perror("open:");
					 return -1;
		  }
		  //   while(1);
			int ret;
		  char buf[128];
		  while(bzero(buf,sizeof(buf)),(ret=read(fdr,buf,sizeof(buf)))>0)
								{
								printf("the ret is %d\n",ret);
								printf("the fdr is %d\n",fdr);
								printf("%s\n",buf);

								}
								return 0;
}
