#include<stdio.h>
#include<pthread.h>
#define NUM 100000000
pthread_mutex_t mutex;
int sum=0;

void *p_func(void *p1)
{
	int i;
	for(i = 0; i < NUM; i++)
	{
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	printf("child sum is %d\n", sum);
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
	for(i = 0; i < NUM; i++)
	{
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	printf("the sum is %d\n", sum);
	pthread_join(pthid,NULL);
	if(0 != ret)
	{
		perror("pthread_join");
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
