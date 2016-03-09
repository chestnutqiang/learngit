#include<stdio.h>

int main(int argc, char *argv[])
{



	int a;
	scanf("%d", &a);
	printf("a is : %d\n", a);
	printf("hello world!\n");
	FILE * fp = fopen(argv[1], "r");
	perror(argv[1]);
	return 0;
}
