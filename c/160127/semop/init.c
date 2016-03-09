#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#define PROJ_ID 1
#define TIMES 100000000
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

	int *p;
	p = (int*)shmat(shmid, NULL, 0);
	if((int*)-1 == p)
	{
		perror("shmat");
		return -1;
	}
	int i;
	*p = 0;
	printf("the value is %d\n", *p);
		
	return 0; 
}
