#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
int main(int argc,char* argv[])//字符指针数组，即二级指针
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}
	char** pptr;
	struct hostent* hptr;//定义结构体指针用于遍历信息
	if((hptr=gethostbyname(argv[1]))==NULL)//if为NULL,访问的域名不存在
	{
		perror("gethostbyname");
		return -1;
	}
	printf("the hname is %s\n",hptr->h_name);
	pptr=hptr->h_aliases;
	int i;
	for(i=0;pptr[i] !=NULL;i++)
	{
		printf("h_aliases is %s\n",pptr[i]);
	}
	printf("AF_INET is %d,h_addrtype is %d\n",AF_INET,hptr->h_addrtype);
	printf("h_length is %d\n",hptr->h_length);
	char str[32];
	pptr=hptr->h_addr_list;
	for(i=0;pptr[i] !=NULL;i++)
	{
		bzero(str,sizeof(str));
		printf("%s\n",inet_ntop(hptr->h_addrtype,pptr[i],str,sizeof(str)));
	}
	return 0;
}

