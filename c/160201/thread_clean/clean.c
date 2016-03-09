#include<stdio.h>
#include<pthread.h>

void cleanup(void *p)
{
	printf("p is %ld\n",(long)p );
}

void *pthread_func(void *p)
{
	pthread_cleanup_push(cleanup, (void*)1);
	pthread_cleanup_push(cleanup, (void*)2);
	sleep(3);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
}

int main()
{
	pthread_t pthid;
	int ret;
	ret = pthread_create(&pthid, NULL, pthread_func,NULL);
	if(0 != ret)
	{
		perror("pthread_create");
		return -1;
	}
	ret=pthread_join(pthid, NULL);
	if(0 != ret)
	{
		perror("pthread_join");
		return -1;
	}
	return 0;
}
