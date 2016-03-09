#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main()
{
	printf("uid=%d, gid=%d\n", getuid(), getgid());
	printf("euid=%d, egid=%d\n", geteuid(), getegid());
	
	FILE *fp = fopen("a.txt", "w");
	if(fp == NULL)
	{
		perror("fopen error");
		exit(-1);
	}
	fputs( "world\n",fp);
	fclose(fp);
	return 0;
}
