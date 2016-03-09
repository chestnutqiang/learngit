#include<stdio.h>

int main()
{
	printf("hello world!\n");
	FILE * fp = fopen("file1", "r");
	perror("file1");
	return 0;
}
