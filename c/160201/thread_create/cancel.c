#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void * pthread_func(void *p)
{
	printf("i am childe thread\n");
	while(1);
	pthread_exit(NULL);
}
int main()
{
	pthread_t pthid;
	int ret;
	pthread_create(&pthid, NULL,pthread_func, NULL);
	ret = 1;
	sleep(3);
	ret = pthread_cancel(pthid);
	if(0 != ret)
	{
		printf("ret is %d\n", ret);
	}
	pthread_join(pthid,NULL);	
	return 0;
}
