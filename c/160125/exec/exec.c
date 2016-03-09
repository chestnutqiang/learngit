#include<stdio.h>
#include<unistd.h>

int main()
{
	execl("./add","add", "2", "5", NULL);
	printf("can't execute here\n");
	return 0;
}
