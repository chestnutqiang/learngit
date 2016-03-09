#include<stdio.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("The args is error!\n");
		return -1;
	}
	int fd;
	fd = open(argv[1], O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	char *p;
	p = mmap(NULL, 100, PROT_READ | PROT_WRITE,MAP_SHARED, fd,0);
	if((void*)p == (void*)-1)
	{
		perror("mmap");
		return -1;
	}
	char *q;
	q = p;
	int cnt = 0;
	while(cnt < 10)
	{
		*q -= 32;
		q = q+1;
		cnt++;
	}
	int ret;
	ret = munmap(p, 100);  //成功时候返回0 失败返回-11
	if(ret < 0)
	{
		perror("munmap");
		return -1;
	}
	close(fd);
	return 0;
}






