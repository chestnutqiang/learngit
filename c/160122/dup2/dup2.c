#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<strings.h>
#include<string.h>
int main(int argc , char *argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
	int fd1;
	fd1 = open(argv[1], O_RDWR, 0600);
	if(-1 == fd1)
	{
		perror("open1");
		printf("errno is %d\n", errno);
		return -1;	
	}
	int fd2;
	fd2 = open(argv[2], O_RDWR,0666);		
	if(-1 == fd2)
	{
		perror("open2");
		printf("errno is %d\n", errno);
		return -1;
	}
	printf("fd1=%d, fd2=%d\n", fd1,fd2);
	int fd3 = dup2(fd1, fd2);
	printf("fd3=%d\n",fd3);
	char buf[128];
	bzero(buf,sizeof(buf));
	read(fd3, buf, sizeof(buf));
	printf("the buf is %s\n", buf);
	return 0;
}




