#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc,char* argv[])
{
	if(argc!=3){
		printf("error args\n");
	}
	int fdw;
	fdw = open(argv[1],O_WRONLY);
	if(-1==fdw)
	{
		perror("open1:");
	}
	int fdr;
	fdr = open(argv[2],O_RDONLY);
	if(-1==fdr)
    {
		 perror("open1:");
	  } printf("fdr=%d,fdw=%d\n",fdr,fdw);

	fd_set readset;
	int ret;
	char buf[128];
	while(2){
       FD_ZERO(&readset);
       FD_SET(0,&readset);
       FD_SET(fdr,&readset);
	 	 ret = select(fdr+1,&readset,NULL,NULL,NULL);
			if (ret>0){
				if(FD_ISSET(0,&readset)){
					bzero(buf,sizeof(buf));
					if((ret=read(0,buf,sizeof(buf)))>0){
						write(fdw,buf,ret-1);
						}else{
								write(fdw,"bye",3);
										break;}}
					if(FD_ISSET(fdr,&readset)){
							bzero(buf,sizeof(buf));
							if(read(fdr,buf,sizeof(buf))>0){
							printf("%s\n",buf);
}
							if(strcmp(buf,"bye")==0)
								break;
}
}
}
close(fdr);
close(fdw);
return 0;
}
					                        
							
		

