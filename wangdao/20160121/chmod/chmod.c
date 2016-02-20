#include <stdio.h>
#include <stdlib.h>

int main(int argc ,char* argv[])
{
	int ret;
	if(argc != 2)
		{
			printf("the system is error\n");
			return -1;
		}
	ret =	chmod(argv[1],0666);
	if(ret == -1)
	{
		perror("chmod failure");
		return -1;	
	}
	return 0;

}
