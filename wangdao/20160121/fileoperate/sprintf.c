#include <stdio.h>
#include <string.h>

int main(int argc,char* argv[])
{
	char* p;
 	p = "I love you";
	char c[40];
	memset(c,0,sizeof(c));
	printf("the argv[0] is %s\n",argv[0]);
	int ret;
	ret = sprintf (c,"%s",p);
	printf("the c is %s\n",c);
	printf("the ret is %d\n",ret);	
	memset(c,0,sizeof(c));
	ret =  sprintf(c,"%s%s",argv[1],argv[0]+1);
	printf("the ret is %d\n",ret);	
	printf("the c is %s\n",c);	
	return ret;
}
