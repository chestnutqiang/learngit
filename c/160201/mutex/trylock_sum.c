#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#define NUM 10000000
pthread_mutex_t mutex;
int sum=0;

void *p_func(void *p1)
{
	int i;
	pthread_mutex_lock(&mutex);
	printf(" I am child\n");
	while(1);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	int ret;
	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		perror("pthread_mutex_init");
		return -1;
	}
	pthread_t pthid;
	pthread_create(&pthid, NULL, p_func,NULL);
	int i;
	sleep(2);
	ret = pthread_mutex_trylock(&mutex);
	printf("the ret is %d\n" , ret);
	pthread_join(pthid,NULL);
	if(0 != ret)
	{
		perror("pthread_join");
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
