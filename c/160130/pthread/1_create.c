#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<strings.h>

void *pthread_entry(void *p)
{
	printf("thread create successful\n");
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t pth_id;
	int ret;
	ret = pthread_create(&pth_id, NULL, pthread_entry,NULL);
	if( -1 == ret)
	{
		perror("pthread_create");
		printf("ret is %d\n", ret);
		return -1;
	}
	sleep(3);
	return 0;

	
}
