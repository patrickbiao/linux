#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		printf("system is error\n");
		return -1;
	}
	int ret ;
  	ret = mkdir(argv[1],0770);
	if (ret == -1)
	{
		perror("the problem is:");
		return -1;
	}
}

