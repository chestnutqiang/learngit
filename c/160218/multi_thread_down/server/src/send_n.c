#include"factory.h"
void send_n(int fd, char *p, int len)
{
	int size, total_len;
	size = 0;
	total_len = 0;
	while(total_len < len)
	{
		size = send(fd, p+total_len, len-total_len, 0);
		total_len += size;
	}
}
