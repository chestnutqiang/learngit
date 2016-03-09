#include"head.h"

void change_nonblock(int *fd)
{
	int ret, status;
	status = fcntl(*fd, F_GETFL);
	status |=  O_NONBLOCK;
	ret = fcntl(*fd, F_SETFL, status);
	if(-1 == ret)
	{
		perror("fcntl");
		return ;
	}
}


int socket_init(char *ip, int port)
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
	ret = bind(sfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	ret = listen(sfd, 128);
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	return sfd;
}
int tcp_accept(int sfd)
{
	int new_fd;
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(struct sockaddr));
	int addrlen = sizeof(struct sockaddr);
	new_fd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);
	if(-1 == new_fd)
	{
		perror("accept");
		return -1;
	}
	printf("%s %d\n success connect\n", inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	return new_fd;
}
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("args");
		return -1;
	}
	int sfd, new_fd, ret;
	sfd = socket_init(argv[1], atoi(argv[2]));
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
	int size;
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
				new_fd = tcp_accept(sfd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = new_fd;
				change_nonblock(&new_fd);
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev) == -1 )
				{
					perror("epoll_ctl: new_fd");
					return -1;
				}
			}
			if(new_fd > 0)
			{
				if(events[i].data.fd==new_fd & events[i].events==EPOLLIN)
				{
					while(1)  //循环读socket缓冲区
					{
						memset(buf, 0, sizeof(buf));  
						size = recv(new_fd, buf, 4, 0);   //边缘触发模式，服务端会阻塞在这里。 需要修改文件描述符的属性。
														  //把描述符的属性修改成非阻塞模式
						if(size > 0)    //读取成功时，返回的是读取的字节数。
						{
							printf("%s", buf);
						}else if(size == -1){//读取失败，返回-1
							break;
						}else{//另一端关闭返回0  
							ev.events = EPOLLIN;
							ev.data.fd = new_fd;
							epoll_ctl(epfd, EPOLL_CTL_DEL, new_fd, &ev);   //注销这个已经关闭的socket文件描述符
						}
					}
					printf("\n");   //最后刷新输出缓冲区
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


	}
	return 0;

}
