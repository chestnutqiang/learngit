#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
	int ret;
	ret = unlink(argv[1]);
	if(ret == -1)
	{
		perror("mkfifo");
		return -1;
	}	
	return 0;
}
