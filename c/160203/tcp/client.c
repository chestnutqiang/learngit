#include"head.h"

int main(int argc, char*argv[])
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
	struct sockaddr_in myaddr;
	bzero(&myaddr, sizeof(struct sockaddr));
	myaddr.sin_family=AF_INET;
	myaddr.sin_port=htons(atoi(argv[2]));
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	ret = connect(sfd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect success\n");
	//用套接字发送数据给server
	char msg[128];
	fd_set set;
	while(1)
	{
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO, &set);
		FD_SET(sfd, &set);
		bzero(msg, sizeof(msg));
		ret = select(15, &set, NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(STDIN_FILENO, &set))
			{
				read(STDIN_FILENO, msg, sizeof(msg));
				ret = send(sfd, msg, strlen(msg)-1, 0);
				if(-1 == ret)
				{
					perror("send");
					close(sfd);
				}
			}
			if(FD_ISSET(sfd, &set))
			{
				ret = recv(sfd, msg, sizeof(msg), 0);
				if(-1 == ret)
				{
					perror("recv");
					close(sfd);
					return -1;
				}
				puts(msg);
			}
		printf("%d \n", ret++);
		}
	}
}
