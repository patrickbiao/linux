#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc,char* argv[])
{
	if(argc !=3)
	{
		printf("error args\n");
		return -1;
	}
	int fdr;
	fdr=open(argv[1],O_RDONLY);
	if(-1==fdr)
	{
		perror("open1");
		return -1;
	}
	int fdw;
	fdw=open(argv[2],O_WRONLY);
	if(-1==fdw)
	{
		perror("open2");
		return -1;
	}
	printf("fdr=%d,fdw=%d\n",fdr,fdw);
	char buf[128];
	int ret;
	fd_set readset;
	while(1)
	{
		FD_ZERO(&readset); //清空readset集合
		FD_SET(STDIN_FILENO,&readset);//设置标准输入到readset集合
		FD_SET(fdr,&readset);//设置fdr到readset集合
		ret=select(fdr+1,&readset,NULL,NULL,NULL);//通过select监控readset里的描述符是否可读,有可读的描述符就返回
		if(ret >0)
		{
			if(FD_ISSET(fdr,&readset)) //如果fdr可读
			{
				bzero(buf,sizeof(buf));
				if(read(fdr,buf,sizeof(buf))>0)
				{
					printf("%s\n",buf);
				}
			}
			if(FD_ISSET(STDIN_FILENO,&readset))//如果标准输入可读
			{
				
				bzero(buf,sizeof(buf));
				if((ret=read(STDIN_FILENO,buf,sizeof(buf)))>0)
				{	
					write(fdw,buf,ret-1);
				}else{
					write(fdw,"bye",3);
					break;
				}
			}				
		}
	}
	close(fdr);
	close(fdw);
	return 0;
}

