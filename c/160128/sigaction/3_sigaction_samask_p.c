#include<stdio.h>
#include<signal.h>
#include<strings.h>
#include<stdlib.h>

void siga(int signum, siginfo_t *p1, void *p2)
{
//	printf("pid=%d, uid=%d \n", p1->si_pid, p1->si_uid);
	printf("signum %d \n", signum);
	sleep(5);
	sigset_t set;
	sigemptyset(&set);
	sigpending(&set);
	if(1 == sigismember(&set, SIGINT))
	{
		printf("the SIGINT is pending\n");
	}
	if(1 == sigismember(&set, SIGQUIT))
	{
		printf("the SIGQUIT is pendit\n");
	}
	printf("afer sleep , sig is %d\n", signum);
}

int main()
{
	struct sigaction act;
	act.sa_sigaction = siga;
	sigemptyset(&act.sa_mask);
	sigfillset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO | SA_NODEFER ;
	int ret;
	ret = sigaction(SIGINT, &act, NULL);
	if(-1 == ret)
	{
		perror("sigaction");
		return -1;
	}
	while(1);
	return 0;	
}
