#include<sys/uio.h>
#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main(int argc, char *argv[])
{

	if(argc != 2)
	{
		perror("args");
		return -1;
	}
	int fd;
	fd = open(argv[1], O_RDWR);
	char buf1[10] = "hello ";
	char buf2[10] = "world";
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 6;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	int ret = writev(fd, iov, 2);
	if(-1 == ret)
	{
		perror("writev");
		return -1;
	}
	close(fd);
	return 0;
}
