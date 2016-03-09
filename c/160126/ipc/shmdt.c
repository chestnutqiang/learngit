#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#define PROJ_ID 1

int main(int argc, char *argv[])
{
	key_t skey;
	skey = ftok(argv[1],PROJ_ID);
	if(-1 == skey)
	{
		perror("ftok");
		return -1;
	}
	printf("the skey is %d",skey);
	int shmid;
	shmid = shmget(skey, 1 << 12, 0600 | IPC_CREAT);
	if( -1 == shmid)
	{
		perror("shmid");
		return -1;
	}	
	printf("the shmid is %d \n", shmid);

	char *p;
	p = (char*)shmat(shmid, NULL, 0);
	if((char*)-1 == p)
	{
		perror("shmat");
		return -1;
	}
	getchar();
	shmdt(p);
	return 0; 
}
