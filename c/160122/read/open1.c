#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<strings.h>
#include<string.h>
int main(int argc, char *argv[])
{
	char buf[40];
	int fd1, fd2;
	fd1=open(argv[1], O_RDONLY);
	fd2=open("filecreat2",O_WRONLY|O_CREAT, 0775);
	if(-1 == fd1)
	{
		perror("fd1");
		return -1;
	}	
	if(-1 == fd2)
	{
		perror("fd2");
		return -1;
	}
	int fw;
	while( bzero(buf, sizeof(buf)), read(fd1, buf, sizeof(buf)) > 0)	
	{
		printf("%s",buf); 
		fw = write(fd2, buf, strlen(buf));
		if(-1 == fw)
		{
			perror("write failed");
			exit(-1);
		}
	}
	close(fd1);
	close(fd2);
	return 0;
}
