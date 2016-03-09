#include"head.h"
void change_nonblock(int *fd)
{
	int ret, status;
	status = fcntl(*fd, F_GETFL);
	status |= O_NONBLOCK;
	ret=fcntl(*fd, F_SETFL);
	if(-1 == ret)
	{
		perror("fcntl");
		return ;
	}
}
int tcp_connect(const char *ip, int port)
{
	int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = inet_addr(ip);
	ret = connect(sfd, (struct sockaddr*)&my_addr,  sizeof(struct sockaddr));
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	return sfd;
}
int main(int argc, char *argv[])
{
	int sfd;
	sfd=tcp_connect(argv[1], atoi(argv[2]));
	printf("connect success!\n");
	int epfd, ret;
	epfd=epoll_create(1);
	struct epoll_event ev, events[5];
	ev.events = EPOLLIN;
	ev.data.fd = STDIN_FILENO;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl:STDIN_FILENO");
		return -1;
	}
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd,&ev);
	if(-1 == ret)
	{
		perror("epoll_ctl:sfd");
		return -1;
	}
	char buf[128];
	int i, nfds;
	int size;
	for(;;)
	{
		nfds = epoll_wait(epfd, events, 5, -1);
		for(i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == STDIN_FILENO && events[i].events == EPOLLIN)
			{
				memset(buf, 0, sizeof(buf));
				ret = read(STDIN_FILENO, buf, sizeof(buf));
				buf[strlen(buf)-1] = '\0';
				if( ret  > 0)
				{
					send(sfd, buf, strlen(buf), 0);
				}
			}
			else
				if(events[i].data.fd == sfd && events[i].events == EPOLLIN)
				{
					memset(buf, 0, sizeof(buf));
					size = recv(sfd, buf, sizeof(buf), 0);
					if(size > 0)
					{
						printf("%s\n", buf);
					}else{
						close(sfd);
						return 0;
					}

				}
		}
	}
	return 0;
}
