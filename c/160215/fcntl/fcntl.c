#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>


void change_nonblock(int *fd)
{
	int status ,ret;
	status = fcntl(*fd, F_GETFL);
	status |=  O_NONBLOCK;
	ret = fcntl(*fd, F_SETFL, status);
	if(-1 == ret)
	{
		perror("fcntl");
		return ;
	}
}

int main()
{
	int fd, ret;
	fd = 0;
	char buf[110];
	change_nonblock(&fd);
	sleep(1);
	ret = read(fd, buf, sizeof(buf));
	printf("ret = %d\n", ret);
	printf("errno = %d\n", errno);
	printf("buf = %s\n", buf);
	return 0;
}
