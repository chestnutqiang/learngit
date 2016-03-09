#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

void sighandler(int iSignNo)
{
	printf("Capture sign no : %d\n", iSignNo);
	sleep(3);
	printf("After sleep, Capture sign no : %d\n", iSignNo);
}

int main()
{
	signal(SIGINT, sighandler);
	char buf[10];
	read(0, buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}
