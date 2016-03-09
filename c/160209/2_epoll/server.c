#include"head.h"
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("args");
		return -1;
	}
	int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);				//socket
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[2]);
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(sfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));   //bind
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	ret = listen(sfd, 128);						//listen
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	int new_fd;
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(struct sockaddr));
	int addrlen = sizeof(struct sockaddr);
	//设置epoll
	int epfd = epoll_create(1);
	struct epoll_event ev, events[10];
	//设置要处理的事件类型
	ev.events = EPOLLIN;
	//设置与要处理的事件相关的文件描述符
	ev.data.fd = sfd;
	//注册epoll啥时间
	epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);
	
	ev.events = EPOLLIN;
	ev.data.fd = STDIN_FILENO;
	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
	char buf[128];
	int nfds, i;
	while(1)
	{
		//nfds是需要处理事件的数目
		nfds = epoll_wait(epfd, events, 10, -1);
		if(-1 == nfds)
		{
			perror("epoll_wait");
			return -1;
		}

		for(i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == sfd)
			{
				new_fd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);
				if(-1 == new_fd)
				{
					perror("accept");
					return -1;
				}
				printf("%s %d\n success connect\n", inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
				ev.events = EPOLLIN;
				ev.data.fd = new_fd;
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev) == -1 )
				{
					perror("epoll_ctl: new_fd");
					return -1;
				}
			}else
				if(events[i].data.fd==new_fd & events[i].events==EPOLLIN)
				{
					memset(buf, 0, sizeof(buf));
					ret = recv(new_fd, buf, sizeof(buf)-1, 0);
					if(ret > 0)
					{
						printf("%s\n", buf);
					}
					
				}else
					if(events[i].data.fd==STDIN_FILENO &events[i].events==EPOLLIN)
					{
						memset(buf, 0, sizeof(buf));
						ret = read(STDIN_FILENO, buf, sizeof(buf));
						if(ret > 0)
						{
							buf[strlen(buf)-1] = '\0';
							if(send(new_fd, buf, strlen(buf), 0) == -1)
							{
								perror("send");
								close(sfd);
								close(new_fd);
								exit(-1);
							}
						}
					}
		}


	}
	return 0;

}
