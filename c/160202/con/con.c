#include<stdio.h>
#include<pthread.h>

int main()
{
	pthread_cond_t cond;
	int ret;
	ret = pthread_cond_init(&cond, NULL);
	if( 0 != ret)
	{
		printf("pthread_cond_init error");
		return -1;
	}
	ret = pthread_cond_destroy(&cond);
	if( 0 != ret)
	{
		printf("pthread_cond_init error");
		return -1;
	}
	return 0;
}
