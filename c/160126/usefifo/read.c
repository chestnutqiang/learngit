#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<strings.h>
int main(int argc, char *argv[])
{
	int ret;
	int fd;
	fd = open(argv[1], O_RDONLY);
	if(-1 == fd)
	{
		perror("oepn");
		return -1;
	}
	char buf[100];
	bzero(buf,sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	if(-1 == ret)
	{
		perror("write");
		return -1;
	}
	printf("%s\n", buf);
	close(fd);
	return 0;
}
