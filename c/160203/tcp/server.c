#include"head.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("args");
		return -1;
	}
	int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[2])); 
	//my_addr.in_addr.s_addr = inet_addr("192.168.168.130");   //ip地址
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);   //系统自动填入IP地址
	int ret;
	//使套接口 与 ip地址和端口 想关联
	ret = bind(sfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	ret = listen(sfd, 10);     //监听这个端口和IP
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	int newfd = -1;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(struct sockaddr);
	bzero(&clientaddr, addrlen);
	char buf[128];
	fd_set set;
	printf("sfd = %d\n", sfd);
	while(1)	
	{
		//初始化set集合
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO,&set);
		FD_SET(sfd, &set);
		bzero(buf, sizeof(buf));
		if(newfd > 0)
		{
			FD_SET(newfd, &set);
		}
		if(select(sfd+4, &set, NULL, NULL ,NULL) > 0 )
		{

			if(FD_ISSET(sfd, &set))
			{
				//接受远程计算机的链接请求
				newfd = accept(sfd,(struct sockaddr*)&clientaddr,&addrlen); 
				printf("arrive to accept\n");
				if(-1 == newfd)
				{
					perror("accept");
					return -1;
				}
				printf("%s %d\nsucess connect\n", inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

			}
			if(FD_ISSET(newfd, &set))
			{
				ret = recv(newfd, buf, sizeof(buf), 0);
				if(-1 == ret)
				{
					perror("recv");
					close(newfd);
					close(sfd);
					return -1;
				}
				puts(buf);
			}
			if(FD_ISSET(STDIN_FILENO, &set))
			{
				read(STDIN_FILENO, buf, sizeof(buf));
				ret=send(newfd, buf, strlen(buf)-1,0);
				if(-1 == ret)
				{
					perror("send");
					close(newfd);
					close(sfd);
				}
			}
		}
	}
}
