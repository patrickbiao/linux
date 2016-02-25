#include "func.h"

void recv_n(int sfd,char* p,int len)//调用时中间数据要强转成char*
{
	int total=0;
	int size=0;
	while(total < len)
	{
		size=recv(sfd,p+total,len-total,0);
		total=total+size;
	}
}

