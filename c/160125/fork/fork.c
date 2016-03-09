#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	pid_t ret = fork();
	if(ret == 0)
	{
		printf("child process id : %d , ppid : %d \n", getpid(), getppid());
	}
	else{
		printf("child process id : %d , ppid : %d \n", getpid(), getppid());
	}
	return 0;
}
