#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

int main(int argc,char* argv[])
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}
	struct stat st1;
	int ret;
	ret=stat(argv[1],&st1);
	if(-1==ret)
	{
		perror("stat");
		return -1;
	}
	printf("%5lu%6x%3d%5d%5d%5ld%15ld\n",st1.st_ino,st1.st_mode,st1.st_nlink,st1.st_uid,st1.st_gid,st1.st_size,st1.st_mtime);
	return 0;
}

