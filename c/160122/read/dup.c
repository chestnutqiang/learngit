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
	struct stat st;
	fd1 = open("file", O_RDWR);
	fstat(fd1, &st);
	printf("file's link is : %u\n", st.st_nlink);
	fd2 = dup(fd1);
	fstat(fd2,&st);
	printf("file's link is : %u\n", st.st_nlink);
	read(fd1, buf, 5);
	puts(buf);
	close(fd1);
	
	printf("file's link is : %u\n", st.st_nlink);
	printf("\n\n\n\n");	
	bzero(buf, sizeof(buf));
	read(fd2, buf, 10);
	puts(buf);
	close(fd2);
	return 0;
	
}
