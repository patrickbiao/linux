include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define max(a,b) ((a>b)?a:b)

int main()
{
        printf("MAX = %d\n",MAX);
        printf("max(2,9) = %d\n",max(2,9));
}

