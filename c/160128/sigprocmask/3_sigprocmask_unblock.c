#include<stdio.h>
#include<signal.h>
int main()
{
	sigset_t set;
	sigemptyset(&set);
	
	sigaddset(&set, SIGINT);
	
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	sleep(3);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
		
	while(1);	
}
