#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	printf("action:\n");
	execl("./add","add","1","2",NULL);
	printf("the result is count\n");
	return 0;
}
