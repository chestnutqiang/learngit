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
	printf("\n");
	close(1);
	fd2 = dup(fd1);
	close(fd1);
	printf("hello world\n");
		
	
	close(fd2);
	return 0;
	
}
