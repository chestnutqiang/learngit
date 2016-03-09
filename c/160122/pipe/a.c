#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<strings.h>
#include<fcntl.h>
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
	int fdr;
	fdr = open(argv[1], O_RDONLY);
	if(-1 == fdr)
	{
		perror("fdr");
		return -1;
	}
	char buf[128];
	int fdw;
	fdw = open(argv[2], O_WRONLY);
	if(-1 == fdw)
	{
		perror("fdw");
		return -1;
	}
	printf("fdr is %d fdw is %d \n", fdr, fdw);
	int ret;
	while(bzero(buf,sizeof(buf)), read(fdr,buf,sizeof(buf)) >0)
	{
		printf("%s\n", buf);
		bzero(buf, sizeof(buf));
		if((ret = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
		{
			write(fdw,buf,ret-1);
		}else
		{
			write(fdw,"bye",3);
			break;
		}	
	}
	close(fdr);
	close(fdw);	
	return 0;
	
}
