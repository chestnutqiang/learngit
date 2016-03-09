#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>

#define PROJ_ID 1

int main(int argc, char *argv[])
{
	int shmid;
	shmid = shmget(IPC_PRIVATE, 1 << 12, 0600 | IPC_CREAT);
	if( -1 == shmid)
	{
		perror("shmid");
	}	
	printf("the shmid is %d \n", shmid);



	return 0; 
}
