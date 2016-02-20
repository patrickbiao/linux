#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		printf("system error\n");
		return -1;
	}
	char buf[50];
	bzero(buf,sizeof(buf));
  	printf ("The current path is:%s\n",getcwd(buf,sizeof(buf)));

	 chdir(argv[1]);

  	printf ("The current path is:%s\n",getcwd(buf,sizeof(buf)));
	printf("buf is %s\n",buf);
}
