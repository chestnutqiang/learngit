#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	int fd[2];
	pipe(fd);
	if(fork() == 0)  //子进程读
	{
		close(fd[1]);
		char buf[100];
		//sleep(2);
		read(fd[0],buf,sizeof(buf));
		printf("read is %s\n", buf);
		close(fd[0]);	
		exit(0);
	}
	close(fd[0]);
	write(fd[1], "hello world", 1333);
	waitpid(-1, NULL, 0);
	close(fd[1]);	 
	exit(0);
}
