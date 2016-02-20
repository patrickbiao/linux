#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>

void myscandir(char* path,int width)
{
		DIR* dir;//typedef struct __dirstream DIR;定义一个结构体指针dir

		dir=opendir(path);//opendir 返回一个指向结构体DIR的指针dir
		if(NULL == dir)
		{
				perror("opendir");
				return;
		}
		struct dirent* pt;
		char buf[128];
		while(pt=readdir(dir))//1.struct dirent *readdir(DIR *dir);readdir返回一个指向struct dirent的结构体指针pt

		{
				if(!strcmp(pt->d_name,".") || !strcmp(pt->d_name,".."))
				{}else{
							  printf("%*s%s\n",width," ",pt->d_name);
							  if(pt->d_type !=4)
							  {
							  }else{
									  bzero(buf,sizeof(buf));//将buf置零
									  sprintf(buf,"%s%s%s",path,"/",pt->d_name);
									  myscandir(buf,width+4);//每级目录用四格隔开
							  }
					  }
		}	
		closedir(dir);
}	


int main(int argc,char* argv[])
{
		if(argc !=2)
		{
				printf("error args\n");
				return 1;
		}
		printf("%s\n",argv[1]);
		myscandir(argv[1],0);
		return 0;
}
