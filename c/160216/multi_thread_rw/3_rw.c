#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
typedef struct thread_args
{
	int rfd;
	int wfd;
	int offset_r;
	int offset_w;
}thread_args_t;

void *func_thread(void *args)
{
	thread_args_t tmp;
	//tmp = *(thread_args_t*)args;
	memcpy(&tmp, (thread_args_t*)args, sizeof(thread_args_t));
	char buf[128];
	int i;
	int ret;
	ret = lseek(tmp.rfd, tmp.offset_r, SEEK_SET);
	if(-1 == ret)
	{
		perror("lseek tmp.rfd");
		exit(-1);
	}
	printf("lseek tmp.rfd = %d\n", ret);

	read(tmp.rfd, buf, 10);
	ret = lseek(tmp.wfd, 0, SEEK_SET);//偏到文件开头
	if(-1 == ret)
	{
		perror("lseek tmp.wfd\n");
	}
	printf("lseek tmp.wfd = %d\n", ret);
	
	for(i = 0; i < 10; i++)
	{
		printf("child thread write\n");
		write(tmp.wfd, buf + i, 1);
		sleep(1);
	}
}
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("args");
		exit(-1);
	}
	int r1, r2, w1, w2;
	r1 = open(argv[1], O_RDONLY);
//	r2 = open(argv[1], O_RDONLY);
	r2 = dup(r1);

	w1 = open(argv[2],O_WRONLY | O_CREAT , 0664);
//	w2 = open(argv[2],O_WRONLY, 0664);
	w2 = dup(w1);
	ftruncate(w1, 20);
	printf("r1 = %d, r2 = %d, w1 = %d , w2 = %d\n",  r1, r2, w1, w2);	
	thread_args_t t_args;
	t_args.rfd = r2;
	t_args.wfd = w2;
	t_args.offset_r = 10;
	t_args.offset_w = 10;
	pthread_t thid;
	pthread_create(&thid, NULL, func_thread, (void*)&t_args);

	char buf[128];
	int ret;
	read(r1, buf, 10);
	ret = lseek(r1, 0, SEEK_CUR);
	printf("after read off r1, ret = %d\n", ret);
	ret = lseek(r1,0, SEEK_SET);
	printf("set to the beginning, ret = %d\n", ret);
	int i;
	for(i = 0; i < 10; i++)
	{
		printf("main thread write\n");
		write(w1, buf + i, 1);
		sleep(1);
	}

	pthread_join(thid, NULL);
	close(r1);
	close(w1);
	close(r2);
	close(w2);
/*	
	//	lseek(w2, 10, SEEK_SET);
	char buf[128];
	read(r, buf, 20);
	printf("%s\n", buf);
	char *p;
	p = buf;
	write(w1, p, 10);
//	write(w2, p+10, 10);
	close(r);
	close(w1);
//	close(w2);
*/
}
