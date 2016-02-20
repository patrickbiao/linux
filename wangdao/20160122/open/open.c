#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char* argv[])
{
		  if (argc != 2)
		  {
					 printf("the system is error\n");
					 return -1;
		  }
		  else {
					 int fd;
					 //fd = open (argv[1],O_RDONLY|O_WRONLY|O_CREAT);不使用


					 fd = open (argv[1],O_RDONLY|O_WRONLY|O_CREAT,0666);
					 if (fd == -1){
								printf ("the error is :%d\n",errno);
								perror("open:");
								return -1;
					 }else
								return 0;
		  }
}	
