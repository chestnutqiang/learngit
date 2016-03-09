#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<strings.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int fd1, fd2;
	fd1 = open("file", O_RDWR);
	fd2 = open("file1",O_RDWR);
	int fd3, fd4;
	fd3 = dup(fd1);
	fd4 = dup2(fd1, fd2);
	printf("fd1:%d fd2:%d fd3:%d fd4:%d\n", fd1, fd2, fd3, fd4);
	read(fd2, buf, sizeof(buf));
	printf("%s\n", buf);
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	return 0;
	
}
