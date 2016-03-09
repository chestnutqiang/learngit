#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutex;
pthread_cond_t cond;
int tickets = 10;
void *func(void *p)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(tickets > 0)
		{
			printf("%ldth window sale ticket,", (long)p);
			tickets--;
			if(tickets == 0) 
			{
				pthread_cond_signal(&cond);
			}
			printf("The rest of the votes : %d\n\n", tickets);
		}else {
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}
void *func1(void *p1)
{
	pthread_mutex_lock(&mutex);
	if(tickets > 0)
		pthread_cond_wait(&cond, &mutex);
	printf("********************************\n");
	printf("reset tickets...\n");
	printf("********************************\n");
	tickets = 10;
	pthread_mutex_unlock(&mutex);
	sleep(1);
	pthread_exit(NULL);
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_t thid[2], set;
	long i;
	for(i = 0; i < 2; i++)
	{
		pthread_create(&thid[i],NULL,func, (void*)(i+1));
	}
	pthread_create(&set, NULL, func1, NULL);
	for(i = 0; i < 2; i++)
	{
		pthread_join(thid[i], NULL);
	}
	pthread_join(set, NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
