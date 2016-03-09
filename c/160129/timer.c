#include<stdio.h>
#include<sys/time.h>
#include<signal.h>
#include<stdlib.h>
#include<time.h>
#include<strings.h>
void sig(int s)
{
	time_t tn;
	time(&tn);
	printf("%s\n", ctime(&tn));
}

int main()
{
	signal(SIGVTALARM, sig);
	strcut itimerval it;
	bzero(&it, sizeof(it));
	it.it_value.tv_sec = 5;
	it.it_interval = 2;
	
}
