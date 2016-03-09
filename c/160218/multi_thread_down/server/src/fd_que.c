#include"fd_que.h"

void fd_que_init(pfd_que_t pq, int capacity)
{
	pq->que_head = NULL;
	pq->que_tail = NULL;
	int ret;
	ret = pthread_mutex_init(&pq->mutex, NULL);
	if(0 != ret)
	{
		printf("pthread_mutex_init error\n");
		exit(-1);
	}
	pq->num = 0;
	pq->que_capacity = capacity;
}

void fd_que_push(pfd_que_t pq, pfd_node_t pfd_node)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->que_head == NULL)
	{
		pq->que_head = pfd_node;
		pq->que_tail = pfd_node;
	}
	else
	{
		pq->que_tail->pnext = pfd_node;
		pq->que_tail = pfd_node;
	}
	pq->num++;
	pthread_mutex_unlock(&pq->mutex);
}

void fd_que_pop(pfd_que_t pq, pfd_node_t *pfd_node)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->num > 0)
	{
		(*pfd_node) = pq->que_head;
		pq->que_head = (*pfd_node)->pnext;
		printf("fd_que_pop : (*pfd_node)->new_fd = %d\n", (*pfd_node)->new_fd);
		pq->num--;
	}
	pthread_mutex_unlock(&pq->mutex);
}

int fd_que_empty(pfd_que_t pq)   //判断空后要解锁
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->num == 0)
	{
		return 1;
	}
	return 0;
}
