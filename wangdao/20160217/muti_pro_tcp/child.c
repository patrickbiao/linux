#include "head.h"

void make_child(pchild cptr,int n)//包括下面的child_handle都需要在头文件中声明
{
	int i;
	int fds[2]; 
	int ret;
	int pid;
	for(i=0;i<n;i++)//for循环创建子进程
	{
		ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//创建之前初始化描述符
		if(-1==ret)
		{
			perror("socketpair");
			return;
		}
		pid=fork();//fork创建进程
		if(pid==0)
		{
			close(fds[1]);  
			child_handle(fds[0]);//子进程已经进来，不会往下执行
		}
		//以下是父进程做的
		close(fds[0]);
		cptr[i].pid=pid;//将其赋给第i个进程的pid
		cptr[i].fds=fds[1];//将写端赋给主进程	
		cptr[i].busy=0;
	}
}

void child_handle(int fdr)
{
	int fd;
	int flag=1;
	while(1)//主进程一直干这个事
	{
		recv_fd(fdr,&fd);//如果父进程没有发生描述符过来，就在这里睡觉（卡住）
		send_file(fd);//再封装一个函数，拿到fd之后给客户端发送文件
		write(fdr,&flag,4);//发送完之后给主进程发送一个整型数结束标志
	}
}

void send_file(int sfd)
{
	int ret;
	data_t buf;//发送数据的结构体
	buf.len=strlen(DOWN_FILE);//得到文件（内容）的长度，括号中本身有“”
	strcpy(buf.buf,DOWN_FILE);//将文件名拷贝过去
	ret=send(sfd,&buf, buf.len+4,0);//总共要发的数据是len+4（内容+int len本身,发送时一次发过去，接收时才需要分成两次发	
	//发送文件名，len没有包含前面的4个字节
	if(-1==ret)
	{
		perror("send1");
		return;
	}
	int fd=open(DOWN_FILE,O_RDONLY);//文件已经存在
	if(-1==fd)
	{
		perror("open");
		return;
	}
	while(bzero(&buf,sizeof(buf)),(buf.len=read(fd,buf.buf,sizeof(buf.buf))) > 0)//循环读文件（fd）,大于0才进去
	{
		send_n(sfd,(char*)&buf,buf.len+4);//每次发时都是结构体取地址发过去，前面的四个字节是标志后面的内容有多长的		
	}
	//当子进程向客户端发送完之后，发送结束符
	bzero(&buf,sizeof(buf));
	int flag=0;//标志用0代替
	buf.len=sizeof(int);//标志用一个整型数
	memcpy(buf.buf,&flag,4);//一个整型数，四个字节
	send(sfd,&buf, buf.len+4,0);//发送结束符	
	close(sfd);
	return; 
}

