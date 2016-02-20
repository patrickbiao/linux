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
	if (argc != 2){
		printf("error args\n");
		return -1;	
	}
	int fdw;
   fdw = open(argv[1],O_WRONLY);
	if (-1 == fdw){
		perror("open:");
 		return -1;
	}
//	printf("the fdw is %d\n",fdw);
//		while(1);

	char buf[128];
	while(bzero(buf,sizeof(buf)),read(0,buf,sizeof(buf))>0)
	{
		printf("%s\n",buf);
 		write (fdw,buf,strlen(buf));
	}
	return 0;
}
