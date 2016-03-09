#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#define PROJ_ID 1

int main(int argc, char *argv[])
{
	key_t skey=ftok(argv[1], PROJ_ID);
	if(skey == -1)
	{
		perror("ftok");
		exit(-1);
	}
	int shmid;
	shmid = shmget(skey, 1 << 12, 0600 | IPC_CREAT);
	if( -1 == shmid)
	{
		perror("shmid");
	}	
	printf("the shmid is %d \n", shmid);

	char *p;
	p=(char*)shmat(shmid, NULL, 0);
	if((char*)-1 == p)
	{
		perror("shmat");
		return -1;
	}
	
	printf("%s\n", p);
	return 0; 
}
