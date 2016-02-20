#include <stdio.h>

void main()
{
	 printf("hello world\n");
	 FILE* fp;
	 fp = fopen("test","r+");
	 perror("fopen");
}
