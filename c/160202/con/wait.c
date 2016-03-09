#include<stdio.h>
#include<pthread.h>
pthread_cond_t cond;
pthread_mutex_t mutex;
void *func(void *p)
{
	int ret;
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond, &mutex);
	printf(" I am child, I am here\n");
	if(0!=ret)
	{
		printf("pthread_cond_wait ret = %d\n", ret);
		return ;
	}
	printf("I am child thread, I am wake\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main()
{
	int ret;
	ret = pthread_cond_init(&cond, NULL);
	if( 0 != ret)
	{
		printf("pthread_cond_init error");
		return -1;
	}
	ret = pthread_mutex_init(&mutex, NULL);
	pthread_t thid;
	pthread_create(&thid, NULL, func, NULL);
	
	sleep(1);
	pthread_mutex_lock(&mutex);
	printf(" I am main thread, i can lock\n");
	pthread_mutex_unlock(&mutex);

	ret=pthread_join(thid, NULL);
	if( 0 != ret)
	{
		printf("pthread_join error");
		return -1;
	}
	ret = pthread_cond_destroy(&cond);
	if( 0 != ret)
	{
		printf("pthread_cond_init error");
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
