#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

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
	printf("%-11x%-2d%-8s%-8s%-3ld%-13.12s%-5s\n",st1.st_mode,st1.st_nlink,getpwuid(st1.st_uid)->pw_name,getgrgid(st1.st_gid)->gr_name,st1.st_size,ctime(&st1.st_mtime)+4,argv[1]);
	return 0;
}

