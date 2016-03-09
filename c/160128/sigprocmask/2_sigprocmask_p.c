#include<stdio.h>
#include<signal.h>
int main()
{
	sigset_t set;
	sigemptyset(&set);
	
	sigaddset(&set, SIGINT);
	
	sigprocmask(SIG_BLOCK, &set, NULL);
	sleep(3);
	sigemptyset(&set);
	sigpending(&set);
	if(1 == sigismember(&set, SIGINT))
	{
		printf("SIGINT is pending\n");
	}	
	while(1);	
}
