#include<sys/select.h>
#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("args is error!\n");
		return -1;
	}
	int fdr, fdw;
	fdw = open(argv[1],O_WRONLY);
	if(-1 == fdw)
	{
		perror("fdw");
		return -1;
	}
	fdr = open(argv[2],O_RDONLY);
	if(-1 == fdr)
	{
		perror("fdr");
		return -1;
	}	
	printf("You have established links with each otheri\n");
	char buf[100];
	fd_set readset; 
	int ret;
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(STDIN_FILENO, &readset);
		FD_SET(fdr, &readset);
		ret = select(fdr+1,&readset, NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(STDIN_FILENO, &readset) > 0)
			{
				bzero(buf, sizeof(buf));
				if((ret = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
				{
					write(fdw, buf, ret-1);
				}else{
					break;
				}		
			}	
			if(FD_ISSET(fdr, &readset) > 0)
			{
				bzero(buf, sizeof(buf));
				if((ret = read(fdr, buf, sizeof(buf))) > 0)
				{
					printf("%s\n", buf);
					if(!strcmp(buf, "bye"))
						break;
				}else{
					break;
				}
		
			}
		}
	}
	close(fdr);
	close(fdw);
	return 0;
}













