#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int a,b;
	char c[30];
	memset(c,0,sizeof(c));
	int d,ret;
	strcpy(c,"123 ,456 sting");
	char *p;
	p = (char*)malloc(20*sizeof(char));
	ret = sscanf(c,"%d%d%s",&a,&b,p); 
	printf("the ret is %d\n",ret);
	printf("the d is %d %d %s\n",a,b,p);
	return ;
}
