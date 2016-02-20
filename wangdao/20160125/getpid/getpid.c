#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
	uid_t euid;
	gid_t egid;
	pid_t pid,ppid;
	pid = getpid();//获取自己的进程
	ppid = getppid();//获取自己的父进程
	euid = geteuid();
	egid = getegid();
	printf("pid=%d,ppid=%d,euid=%d,egid=%d\n",pid,ppid,euid,egid);
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	int fd = open(argv[1],O_RDWR);
	if(-1==fd){
		perror("open:");
		return -1;
	}
	int ret;
	ret = write(fd,"world",5);
	if(ret<0)
	{
		perror("write:");
		return -1;
	}
	return 0;
	
}
	
