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
	char buf[128];
	bzero(buf, sizeof(buf));
	ret=sendto(sfd, "i am client", 11, 0, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("sendto");
		return -1;
	}
	struct sockaddr_in client;
	int slen;
	slen = sizeof(struct sockaddr);
	bzero(buf, sizeof(buf));
	bzero(&client, sizeof(struct sockaddr));
	ret = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &slen);
	if(-1 == ret)
	{
		perror("recvfrom");
		return -1;
	}
	printf("i am client , i receive %s", buf);
	close(sfd);
}
