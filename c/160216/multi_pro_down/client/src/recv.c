#include"head.h"

void recv_n(int fd, char *buf, int len)
{
	int size;
	int recv_total;
	size = 0;
	recv_total = 0;
	while(recv_total < len)
	{
		size = recv(fd, buf+recv_total, len-recv_total, 0);
		if(size == -1)
		{
			perror("recv_n:recv");
		}
		recv_total += size;
	}
}
