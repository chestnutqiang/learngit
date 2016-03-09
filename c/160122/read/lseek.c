#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<strings.h>
#include<string.h>
int main()
{
	int fd = open("file2", O_WRONLY);
	lseek(fd, 1024, SEEK_SET);
	write(fd, "m", 1);
	close(fd);
	return 0;		
}
