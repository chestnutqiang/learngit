#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void *threadfunc(void *args)
{
	int i;
	for(i = 0; i < 10; i++)
	{
		printf("I am child thread : %ld : %d\n", (long)args, i);
		sleep(1);	
	}
	while(1);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thid;
	pthread_create(&thid, NULL, threadfunc, (void*)333);
	long i;
	for(i = 0; i < 10; i++)
	{
		printf("i am main thread : %ld\n", thid);
		sleep(1);
	}
	while(1);
	return 0;
}
