#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	char buf[33];
	int fd = open("file", O_WRONLY);
	int fd1 = fd;
//	close(fd);
	printf("read:%d\n", read(fd, buf,sizeof(buf)-1));
	close(fd1);
	return 0;

}
