#ifndef __FACTORY__
#define __FACTORY__

#include"head.h"
#include"fd_que.h"
#define DOWN_FILE "file"
typedef void* (*pfunc)(void *);
typedef struct thread_pool
{
	pthread_cond_t cond;   //条件变量，和主线程通信
	fd_que_t fd_q;			//socket描述符队列，和client通信
	pthread_t *ptid;			//线程id数组，唯一标识每个线程
	pfunc entry;			//线程的入口函数
	int thread_num;			//线程池的线程的数量
	int flag;				//线程是否启动的标识
}thread_pool_t, *pthread_pool_t;

typedef struct data_t
{
	int len;
	char t_data[1000];
}data_t;
void thread_pool_init(pthread_pool_t, int, int, pfunc);
void thread_pool_start(pthread_pool_t);
char* get_conf_value(char *, char *, char *);
void* thread_handle(void*);
void send_file(int);
void send_n(int, char*, int);

#endif
