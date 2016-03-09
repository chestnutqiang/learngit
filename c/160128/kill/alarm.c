#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void sig(int s)
{
	printf("the signum is %d\n",s);
}

int main()
{
	signal(SIGALRM,sig);//设置2号信号的信号处理行为
	int ret;
	alarm(5);
	while(1);
	return 0;
}
