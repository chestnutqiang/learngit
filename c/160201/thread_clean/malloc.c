#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
void cleanup(void *p)
{
	free(p);
	p=NULL;
	printf("the memory was free\n");
}


void *pthread_func(void *p1)
{
	char *p = (char*)malloc(10);

	pthread_cleanup_push(cleanup, p);
	sleep(3);
	pthread_exit(NULL);
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
	pthread_join(pthid, NULL);
	//ret = pthread_cancel(pthid);
	return 0;
}
