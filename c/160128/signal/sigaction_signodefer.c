#include<stdio.h>
#include<strings.h>
#include<signal.h>
#include<stdlib.h>

void siga(int signum, siginfo_t *p, void *p1)
{
	printf("I get the sig %d\n", signum);
	sleep(3);
	printf("after handle , the sig is %d \n", signum);
}

int main()
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_sigaction = siga;
	sigset_t sa_mask;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO |SA_NODEFER;
	int ret;
	ret = sigaction(SIGINT, &act, NULL);
	if(-1 == ret)
	{
		perror("sigaction");
		return -1;
	}	
	ret = sigaction(SIGQUIT, &act, NULL);
	if(-1 == ret)
	{
		perror("sigaction");
		return -1;
	}

	while(1);
}
