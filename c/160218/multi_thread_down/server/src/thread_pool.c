#include"factory.h"

void thread_pool_init(pthread_pool_t tp, int num, int capacity, pfunc entry)
{
	int ret;
	ret = pthread_cond_init(&tp->cond,NULL);
	if( 0 != ret)
	{
		printf("pthread_cond_init error\n");
		exit(-1);
	}
	fd_que_init(&tp->fd_q, capacity);
	tp->ptid = (pthread_t*)calloc(1, sizeof(pthread_t));   //为每一个pthread数组分配空间
	tp->entry = entry;
	tp->thread_num = num;						//设置线程池的线程数量
	tp->flag = 0;								 //设置线程未启动
}
void thread_pool_start(pthread_pool_t tp)
{
	int i, ret;
	if(tp->flag == 0)
	{
		for(i = 0; i < tp->thread_num; i++)
		{
			ret=pthread_create(&tp->ptid[i], NULL, tp->entry, (void*)tp);
			if(0 != ret)
			{
				printf("%d : pthread_create error\n", i);
				exit(-1);
			}
		}
	}
	tp->flag = 1;
}
