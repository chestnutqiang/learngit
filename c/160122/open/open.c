#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<strings.h>
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		perror("args");
		return -1;	
	}
	int fd;
	//fd = open(argv[1], O_RDWR|O_CREAT);//b不使用
	fd = open(argv[1], O_RDWR,0666);
	if(-1 == fd)
	{
		perror("open");
		printf("%d\n", errno);
	}
	char buf[128];
	bzero(buf,sizeof(buf));
	int ret;
	ret = read(fd,buf, sizeof(buf));
	if(-1 == ret)
	{
		perror("read");
		close(fd);
		return -1;
	}
	printf("%s\n", buf);
	return 0;
	
}
