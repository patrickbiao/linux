#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define DOWN_FILE "file"//宏定义下载的文件名
typedef struct pro_child{//子进程信息数据结构
	pid_t pid;
	int fds;//socket的一端，即可以读，又可以写
	int busy;
}child,*pchild;


typedef struct tdata{//每次传数据要用tdata来传
	int len;
	char buf[1000];
}data_t,*pdata_t;

void make_child(pchild,int);
void child_handle(int);
void send_file(int);
void send_fd(int,int);
void recv_fd(int,int*);
void send_n(int,char*,int);

