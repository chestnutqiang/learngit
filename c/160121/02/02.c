#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int main()
{
	printf("The type of file is : %d\n", stat());
	return 0;
}
