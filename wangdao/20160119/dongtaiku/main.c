#include <stdio.h>
#include <stdlib.h>


void main(int argc,char *argv[])
{
	if (argc != 3)
		{
			printf("the system is error\n");
		   return ;
		}
		int a,b;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	printf("a+b is %d",add1(a,b));
}		
