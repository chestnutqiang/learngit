#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("args error!\n");
		return 1;
	}
	mkdir(argv[1], 0707);
	return 0;	
}
