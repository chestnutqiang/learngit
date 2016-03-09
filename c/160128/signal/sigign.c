#include<stdio.h>
#include<signal.h>

int main()
{
	typedef void (*sighandler)(int);
	sighandler p;
	p  = signal(SIGINT,SIG_IGN);
	if(p  == SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	while(1);
}
