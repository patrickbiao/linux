#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

/*因为socketpair初始化的描述符sfd为整型*/
void send_fd(int sfd,int fd)//直接将fd拿出来用，不需要改变，所以不用传指针
{
	struct msghdr msg;//用于存放信息的结构体，包含cmsghdr
	bzero(&msg,sizeof(struct msghdr));
	char buf1[10]="hello";
	char buf2[10]="world";
	struct iovec iov[2];//使用writev前定义一个结构体数组
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;//控制结构体
	int len=CMSG_LEN(sizeof(int));//用该函数来算出结构体长度
	cmsg=(struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int *fdptr;
	fdptr=(int*)CMSG_DATA(cmsg);//算出存放结构体的数据的地址
	*fdptr=fd;//将地址解引用存放fd值
	msg.msg_control = cmsg;//将结构体指针赋值过去
	msg.msg_controllen = len;//填结构体长度
	int ret=sendmsg(sfd,&msg,0);//将描述符fd（open打开的）对应的结构体信息通过socketpair初始化的发送套接字将结构体信息msg发送过去
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}

void recv_fd(int sfd,int* fd)
{
	struct msghdr msg;//定义结构体来存放描述符fd对应的结构体信息
	bzero(&msg,sizeof(struct msghdr));
	char buf1[10]={0};
	char buf2[10]={0};
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(int));//fd是整型值，所以括号直接传入一个整型的长度
	cmsg=(struct cmsghdr *)calloc(1,len);//申请一个长度为len的内存
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret=recvmsg(sfd,&msg,0);//通过socketpair初始化的接收句柄sfd读取描述符fd（open打开的）的结构体信息msg
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);//将存放结构体数据的地址解引用赋给fd，这时这里的fd继承了上面的描述符fd的结构体信息
//	printf("the buf1 is %s\n",buf1);
}


int main(int argc,char** argv)
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	int fds[2];
	int ret;
	ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
	if(-1==ret)
	{
		perror("socketpair");
		return -1;
	}
	if(!fork())//子进程读
	{
		close(fds[1]);//关闭写
		int fd;//该fd是随便定义的一个整型数
		recv_fd(fds[0],&fd);//封装一个函数读fd,从fds[0]中读
		char buf[10]="0";
		read(fd,buf,sizeof(buf));
		printf("I am child,fd is %d\n",fd);//由于子进程关闭了写端，故这里fd为4
		printf("fds[0] is %d\n",fds[0]);
		printf("The buf is %s\n",buf);
		exit(0);
	}else{
			close(fds[0]);//父进程关闭读
			int fd = open(argv[1],O_RDWR);//fds[0]被关，故这里返回的fd为3
			send_fd(fds[1],fd);//封装一个函数往fds[1]写描述符
			printf("I am father,fd is %d\n",fd);//父进程关闭了读端，故这里fd为3（将空闲的分配给它）
			printf("fds[1] is %d\n",fds[1]);
			wait(NULL);
			return 0;
	}
}
			
