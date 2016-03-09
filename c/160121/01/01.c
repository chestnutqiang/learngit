#include<stdio.h>
#include<unistd.h>
int main()
{
	chdir("/home/lizhiqiang/160121/02");
	printf("current working directory:%s\n",getcwd(NULL,0));
	return 0;
}
