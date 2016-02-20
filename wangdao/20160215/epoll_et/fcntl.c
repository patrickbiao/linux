#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

void change_nonblock(int* fd)//由于是fd被F_GETFL作用，所以传地址
{
	int status;
	status=fcntl(*fd,F_GETFL);//解引用,得fd状态
	status=status|O_NONBLOCK;
	int ret;
	ret=fcntl(*fd,F_SETFL,status);//设置fd状态
	if(-1==ret)
	{
		perror("fcntl");
		return ;
	}

}
int main()
{
	char buf[10]={0};
	int ret;
	int fd=0;
	change_nonblock(&fd);//这时标准输入描述符fd就被修改为非阻塞
	ret=read(0,buf,sizeof(buf));
	printf("%s\n",buf);
	printf("errno is %d\n",errno);
	printf("ret is %d\n",ret);

