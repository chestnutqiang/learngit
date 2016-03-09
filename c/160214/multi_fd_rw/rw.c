#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("args");
		exit(-1);
	}
	int r, w1, w2, thid;
	r = open(argv[1], O_RDONLY);
	w1 = open(argv[2],O_WRONLY | O_CREAT , 0664);
	w2 = open(argv[2],O_WRONLY, 0664);
	ftruncate(w1, 20);
	lseek(w2, 10, SEEK_SET);
	char buf[128];
	read(r, buf, 20);
	printf("%s\n", buf);
	char *p;
	p = buf;
	write(w1, p, 10);
	write(w2, p+10, 10);
	close(r);
	close(w1);
	close(w2);
}
