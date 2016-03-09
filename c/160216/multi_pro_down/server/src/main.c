#include"head.h"

int main(int argc, char *argv[])
{
	if(4 != argc)
	{
		perror("args");
		exit(-1);
	}
	pchild child_ptr;
	int num_child;
	num_child = atoi(argv[3]);
	child_ptr = (pchild)calloc(num_child,sizeof(child)); 
	make_child(child_ptr, num_child);  //创建子线程
	int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		exit(-1);
	}
	int ret;
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(struct sockaddr));
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_family = AF_INET;
	ret = bind(sfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		exit(-1);
	}
	ret = listen(sfd, num_child+1);
	if(-1 == ret)
	{
		perror("listen");
		exit(-1);
	}
	int epfd;
	epfd = epoll_create(1);
	if(-1 == epfd)
	{
		perror("epoll_create");
		exit(-1);
	}
	struct epoll_event ev, *evs;
	evs = (struct epoll_event*)calloc(num_child, sizeof(struct epoll_event));
	bzero(&ev, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl:ev");
		exit(-1);
	}
	int i;
	for(i = 0; i < num_child; i++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = child_ptr[i].sfd;
		ret = epoll_ctl(epfd, EPOLL_CTL_ADD,child_ptr[i].sfd, &ev);
		if(-1 == ret)
		{
			printf("epoll_ctl:evs[%d] error\n", i);
			exit(-1);
		}
	}
	int nfds;
	int new_fd;
	int j;
	int cnt;
	cnt = 0;
	int flag, size;

	while(1)
	{
		nfds=epoll_wait(epfd, evs, num_child+1, -1);
		for(i = 0; i < nfds; i++)
		{
			if(evs[i].data.fd == sfd && evs[i].events == EPOLLIN)
			{
				new_fd = accept(sfd, NULL, NULL);
				if(-1 == new_fd)
				{
					perror("accept");
					close(sfd);
					exit(-1);
				}
				printf("%d client have connect to server,new_fd is %d\n", ++cnt,new_fd);
				for(j = 0; j < num_child ; j++)
				{
					if(child_ptr[j].busy == 0)
					{
						send_fd(child_ptr[j].sfd, new_fd);
						child_ptr[j].busy = 1;
						break;
					}
				}
			}
			for(j = 0; j < num_child; j++)  //子进程有数据发来
			{
				if(evs[i].data.fd == child_ptr[j].sfd && evs[i].events == EPOLLIN)
				{
				//	size = read(child_ptr[j].sfd , &flag, 4);
				//	printf("size is %d\n", size);
					child_ptr[j].busy = 0;
				//	printf("%d child is not busy\n", j);
					break;
				}
			}
		}
	}

}
