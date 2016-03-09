#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	pid_t pid = fork();
	if(pid == 0)
	{
		printf(" I am parent process\n");
	}
	else
	{
		//sleep(10);
		while(1);
	}
	return 0;
}
