#include <stdio.h>

int mul (int a,int b);

void main(int argc, char* argv[])
{
		  if(argc != 3)
		  {
					 printf("error args\n");
					 return;
		  }
		  int a,b;
		  a = atoi(argv[1]);
		  b = atoi(argv[2]);
		  printf("the a*b is %d\n",mul(a,b));
}	
