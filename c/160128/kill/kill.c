#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void sig(int s)
{
	printf("signum is %d\n", s);
}

int main()
{
	int ret;	
	signal(SIGUSR1, sig);
	ret  = kill(getpid(), SIGUSR1);
	if(-1 == ret)
	{
		perror("kill");
	}	
	while(1);
}
