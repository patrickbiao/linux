#include "head.h"
/*因为socketpair初始化的描述符sfd为整型*/
void send_fd(int sfd,int fd)//直接将fd拿出来用，不需要改变，所以不用传指针
{
	struct msghdr msg;//用于存放信息的结构体，包含cmsghdr
	bzero(&msg,sizeof(struct msghdr));
	char buf1[10]="hello";
	char buf2[10]="world";
	struct iovec iov[2];//使用writev前定义一个结构体数组,iov必须得赋值，不然发不过去
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
	*fdptr=fd;//将描述符fd放在存放结构体数据的区域
	msg.msg_control = cmsg;//将结构体指针赋值过去
	msg.msg_controllen = len;//填结构体长度
	int ret=sendmsg(sfd,&msg,0);//将描述符fd（open打开的）对应的结构体信息通过socketpair初始化的发送套接字sfd将结构体信息msg发送过去
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


