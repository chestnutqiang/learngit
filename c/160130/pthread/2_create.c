#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<strings.h>

void *pthread_entry(void *p)
{
//	printf("thread create successful\n");
	int i;
	for(i = 0; i < 10; i++)
	{
		printf("Hi, I'm child thread, arg is:%d\n", (int)p);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t pth_id;
	int ret;
	ret = pthread_create(&pth_id, NULL, pthread_entry,(void*)123);
	if( -1 == ret)
	{
		perror("pthread_create");
		printf("ret is %d\n", ret);
		return -1;
	}
	int i;	
	for(i = 0; i < 10; i++)
	{
		printf("Hi, I'm main thread , child thread is:%d\n", i);
		sleep(1);
	}
	sleep(3);
	return 0;

	
}
