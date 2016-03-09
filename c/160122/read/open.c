#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		perror("args");
		return -1;	
	}
	int fd;
	fd = open(argv[1], O_RDWR|O_CREAT,0777);
	if(-1 == fd)
	{
		perror("open");
		printf("%d\n", errno);
	}
	printf("%d\n", errno);
		
	return 0;
	
}
