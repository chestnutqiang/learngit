#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
char *p;
void *thread_func(void *p1)
{

	printf(" I am child\n");  //注意输出刷新缓冲区
	p = (char*)malloc(10);
	//free(p);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thid;
	pthread_create(&thid, NULL, thread_func, NULL);
	int ret;
	ret = pthread_join(thid, NULL); // 填入的thid必须是有效的
	if( 0 != ret)
	{
		perror("pthread_join");
		printf("ret is %d\n", ret);
		return -1;
	}
	strcpy(p, "hello world!");
	printf("malloc is %s",p);
	return 0;
}
