#include<stdio.h>
#include<unistd.h>

int main()
{
	printf("mypid=%d,mypgid=%d\n",getpid(),getpgid(0));
	return 0;
}
