#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
int main(int argc,char* argv[])
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}
	int fd;
	//fd=open(argv[1],O_RDWR|O_CREAT);  不使用
	fd=open(argv[1],O_WRONLY|O_CREAT,0600);
	if(-1==fd)
	{
		perror("open");
		printf("errno is %d\n",errno);
		return -1;
	}
	printf("\n");
	close(STDOUT_FILENO);
	int fd1=dup(fd);
	if(-1==fd1)
	{
		perror("dup");
		close(fd);
		return -1;
	}
	close(fd);
	printf("hello world\n");
	close(fd1);
	return 0;
}

