#include<stdio.h>
#include<pthread.h>

int main()
{
	pthread_mutex_t mutex;
	int ret;
	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		perror("pthread_mutex_init");
		return -1;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
