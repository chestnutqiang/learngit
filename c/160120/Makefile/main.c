#include<stdio.h>
#include"mul.h"
int main(int argc, char *argv[])
{
	int a, b;
	if(argc != 3)
	{
		printf("error args\n");
		return 0;
	}		
	a = atoi(argv[2]);
	b = atoi(argv[1]);
	printf("%d\n", mul(a,b));
	return 0;
}
