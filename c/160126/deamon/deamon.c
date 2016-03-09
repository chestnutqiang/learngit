#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
	int i;
	if(!fork())
	{
		sleep(3);
		exit(0);
	}
	setsid();
	chdir("/");
	umask(0);
	for(i = 0; i < 3; i++)
	{
		close(i);
	}
	while(1);
}
