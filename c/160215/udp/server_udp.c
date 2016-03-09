#include"head.h"

int main(int argc, char *argv[])
{
	int sfd;
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret = bind(sfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	printf("bind ok\n");
	char buf[128];
	struct sockaddr_in client;
	int clen;
	clen = sizeof(struct sockaddr);
	bzero(buf, sizeof(buf));
	bzero(&client, sizeof(struct sockaddr_in));
	ret = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &clen);
	if(-1 == ret)
	{
		perror("recvfrom");
		return -1;
	}
	printf("i am server, i receive %s\n", buf);
	ret=sendto(sfd, "i am server", 11, 0, (struct sockaddr*)&client, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("sendto");
		return -1;
	}
	close(sfd);

}
