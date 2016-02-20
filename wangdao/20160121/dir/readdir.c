#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc,char* argv[])
{
	if(argc !=2)
	{
		printf("error args\n");
		return 1;
	}
	DIR* dir;
	dir=opendir(argv[1]);
	if(NULL == dir)
	{
		perror("opendir");
		return -1;
	}
	struct dirent* pt;
	
	while(pt=readdir(dir))
	{
		printf("inode=%lu,filelength=%hu,type=%d,name=%s\n",pt->d_ino,pt->d_reclen,pt->d_type,pt->d_name);
	}
	closedir(dir);
	return 0;
}

