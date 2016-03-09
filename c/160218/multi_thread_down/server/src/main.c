#include"factory.h"
int main(int argc, char *argv[])
{
	int num, capacity ;
	char path[]="../conf/config";
	char args[128],temp[128];
	bzero(temp, sizeof(temp));
	strcpy(temp, "thread_num");
	//printf("%s\n",get_conf_value(path,temp,args));
	//num = atoi(get_conf_value(path,"pthread_num",args));
	//capacity = atoi(get_conf_value(path,"capacity", args));
	num = 5;
	capacity = 5;
	//printf("num = %d, capacity = %d\n", num, capacity);
	thread_pool_t tp;
	thread_pool_init(&tp, num, capacity, thread_handle);
	thread_pool_start(&tp);
	//tcp连接
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	//ser.sin_port=htons(atoi(get_conf_value(path,"port",args)));
	ser.sin_port = htons(atoi("3000"));
	//ser.sin_addr.s_addr=inet_addr(get_conf_value(path,"ip",args));
	ser.sin_addr.s_addr = inet_addr("192.168.168.130");
	int ret;
	printf("ip : %s , port : %d \n", inet_ntoa(ser.sin_addr), ntohs(ser.sin_port));
	ret=bind(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,capacity);
	int epfd=epoll_create(1);
	struct epoll_event ev,evs[2];
	ev.events=EPOLLIN;
	ev.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	int new_fd;
	while(1)
	{
		ret=epoll_wait(epfd,evs,2,-1);
		if(ret > 0)
		{
			if(evs[0].data.fd == sfd && evs[0].events == EPOLLIN)
			{
				new_fd = accept(sfd, NULL, NULL);
				if(-1 == new_fd)
				{
					perror("accept");
					return -1;
				}
				printf("connect successful\n");
				pfd_node_t pnew_node;
				pnew_node = (pfd_node_t)calloc(1,sizeof(fd_node_t));
				pnew_node->new_fd = new_fd;
				printf("main new_fd = %d\n", new_fd);
				fd_que_push(&tp.fd_q, pnew_node);
				printf("after fd_que_push\n");
				pthread_cond_signal(&tp.cond);
				printf("after pthread_cond_signal\n");
			}
		}
	}
}

void* thread_handle(void *args)
{
	
	pthread_pool_t tp_ptr;
	tp_ptr = (pthread_pool_t)args;
	printf("the start_flag is %d\n", tp_ptr->flag);
	pfd_que_t pq;
	pq = &tp_ptr->fd_q;
	if(fd_que_empty(pq))   //队列等于空，挂起
	{
		//printf("before pthread_cond_wait \n");
		pthread_cond_wait(&tp_ptr->cond,&pq->mutex);  //解锁 挂起 唤醒 加锁
	}
	//队列不等于空，出对一个socket描述符然后发送文件
	//printf("after pthread_cond_wait \n");
	pthread_mutex_unlock(&pq->mutex); //判断空条件没有解锁,所以这里必须解锁
	pfd_node_t pcur_node;
	fd_que_pop(pq, &pcur_node);
	printf("thread_handle pcur_node : %d\n", pcur_node->new_fd);
	send_file(pcur_node->new_fd);
	printf("after sendfile \n");
	free(pcur_node);
}
