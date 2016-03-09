#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		perror("args");
		return -1;
	}
	int fds[2];
	pipe(fds);
	if(!fork())
	{
		//子进程关闭写端
		close(fds[1]);
		int fd;
		char buf[128];
		read(fds[0], &fd, 4);
		printf("I am child process, fd = %d\n", fd);
		read(fd, buf, sizeof(buf));
		printf("buf = %s\n", buf);
		exit(0);
	}
	else
	{
		//父进程关闭读端。
		close(fds[0]);
		int fd = open(argv[1], O_RDWR);
		if(-1 == fd)
		{
			perror("open");
			exit(-1);
		}
		printf("I am parent process, fd = %d\n", fd);
		write(fds[1], &fd, 4);
		wait(NULL);
		close(fd);
		close(fds[1]);
	}
}
