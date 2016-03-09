#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<strings.h>
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
	int fdw;
	fdw = open(argv[1], O_WRONLY);
	if(-1 == fdw)
	{
		perror("fdw");
		return -1;
	}
	int fdr;
	fdr = open(argv[2], O_RDONLY);
	if(-1 == fdr)
	{
		perror("fdr");
		return -1;
	}
	printf("fdr is %d fdw is %d\n", fdr, fdw);	
	char buf[50];	
	int ret;
	while(bzero(buf,sizeof(buf)),(ret = read(STDIN_FILENO,buf,sizeof(buf)))>0)
	{
		write(fdw, buf, ret-1);	
		bzero(buf,sizeof(buf));
		if((read(fdr, buf, sizeof(buf))) > 0 )
		{
			printf("%s\n", buf);
			if(strcmp(buf, "bye") == 0 )
			break;
		}
	}
	close(fdr);
	close(fdw);
	return 0;	
		
}





