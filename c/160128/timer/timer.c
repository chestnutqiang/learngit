#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<signal.h>
#include<strings.h>
#include<stdlib.h>
#include<time.h>
void  handler(int s)
{
	time_t tm;
	time(&tm);
	printf("%s\n\n", ctime(&tm));
}
int main()
{
	signal(SIGALRM, handler);
	struct itimerval it;
	bzero(&it, sizeof(it));
	it.it_interval.tv_sec = 4;
	it.it_value.tv_sec = 3;
	int ret;	
	ret = setitimer(ITIMER_REAL, &it, NULL);
	if(-1 == ret)
	{
		perror("setitimer");
		return -1;
	}
	while(1);
}
