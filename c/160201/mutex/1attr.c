#include<stdio.h>
#include<string.h>
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
	pthread_mutex_lock(&mutex);
	printf(" I am child\n");
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	int ret;
	pthread_mutexattr_t attr;
	int t;
	t =  PTHREAD_MUTEX_RECURSIVE_NP;
	//attr.__mutexkind=PTHREAD_MUTEX_RECURSIVE_NP;
	memcpy(&attr, &t, 4);
	ret = pthread_mutex_init(&mutex, &attr);
	if(0 != ret)
	{
		perror("pthread_mutex_init");
		return -1;
	}
	pthread_t pthid;
	pthread_create(&pthid, NULL, p_func,NULL);
	pthread_join(pthid,NULL);
	if(0 != ret)
	{
		perror("pthread_join");
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
