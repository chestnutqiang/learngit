#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
void *thread_func(void *p)
{
	printf(" I am child\n");  //注意输出刷新缓冲区
	sleep(3);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thid;
	pthread_create(&thid, NULL, thread_func, NULL);
	int ret;
	ret = pthread_join(-1, NULL); // 填入的thid必须是有效的
	if( 0 != ret)
	{
		perror("pthread_join");
		printf("ret is %d\n", ret);
		return -1;
	}
	return 0;
}
