#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("args error!\n");
	}
	rmdir(argv[1]);
	return 0;
}
