#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		printf("I am child\n");
		exit(0);	
	}else{
		int status;
		pid_t ret;
		ret = wait(&status);
		if(ret < 0)
		{
			perror("wait");
			return -1;
		}
		if(WIFEXITED(status))
		{
			printf("the exit value is %d \n", status);
		}
		return 0;	
	}
}
