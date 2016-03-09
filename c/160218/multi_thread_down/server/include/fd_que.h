#ifndef __FD_QUE__
#define __FD_QUE__
#include"head.h"
typedef struct fd_node   //描述符的队列节点
{
	int new_fd;
	struct fd_node *pnext;
}fd_node_t,*pfd_node_t;   

typedef struct fd_que  //描述符的队列
{
	pfd_node_t que_head, que_tail;
	pthread_mutex_t mutex;
	int num;      //队列当前存放描述符的数量
	int que_capacity;  //队列能容纳的最大描述符数量
}fd_que_t, *pfd_que_t;
void fd_que_init(pfd_que_t, int);
void fd_que_push(pfd_que_t, pfd_node_t);
void fd_que_pop(pfd_que_t, pfd_node_t*);
int fd_que_empty(pfd_que_t);
#endif
