#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	printf("uid=%d,gid=%d,euid=%d,egid=%d\n", getuid(),getgid(),geteuid(),getegid());
	while(1);
	return 0;
}
