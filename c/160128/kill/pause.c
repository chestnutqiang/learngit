#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
void sig(int s)
{
	printf("the signum is %d\n",s);
}

int main()
{
	signal(SIGALRM,sig);//设置2号信号的信号处理行为
	pause();
	char buf[10];
	fgets(buf,sizeof(buf),stdin);
	printf("%s\n", buf);
	return 0;
}
