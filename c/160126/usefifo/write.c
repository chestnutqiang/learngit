#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int ret;
	ret = mkfifo(argv[1], 0664);
	if(-1 == ret)
	{
		perror("mkfifo");
		return -1;
	}
	int fd;
	fd = open(argv[1], O_WRONLY);
	if(-1 == fd)
	{
		perror("oepn");
		return -1;
	}
	char buf[40];
	fgets(buf,sizeof(buf), stdin);
	ret=write(fd,buf, strlen(buf));	
	if(-1 == ret)
	{
		perror("write");
		return -1;
	}
	close(fd);
	unlink(argv[1]);
	return 0;
}
