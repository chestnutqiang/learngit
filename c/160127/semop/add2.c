#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<strings.h>
#define PROJ_ID 1
#define TIMES 10000000
int main(int argc, char *argv[])
{
	key_t skey;
	skey = ftok(argv[1],PROJ_ID);
	if(-1 == skey)
	{
		perror("ftok");
		return -1;
	}
//	printf("the skey is %d",skey);
	int shmid;
	shmid = shmget(skey, 1 << 12, 0600 | IPC_CREAT);
	if( -1 == shmid)
	{
		perror("shmid");
		return -1;
	}	
//	printf("the shmid is %d \n", shmid);

	int *p;
	p = (int*)shmat(shmid, NULL, 0);
	if((int*)-1 == p)
	{
		perror("shmat");
		return -1;
	}
	int semid;
	semid = semget((key_t)2334, 0, 0600 | IPC_CREAT);
	int i;
	struct sembuf sop;
	bzero(&sop, sizeof(sop));
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = SEM_UNDO;
	
	struct sembuf sov;
	bzero(&sov, sizeof(sov));
	sov.sem_num = 0;
	sov.sem_op = 1;
	sov.sem_flg = SEM_UNDO;
	for(i = 0; i < TIMES; i++)
	{
		semop(semid, &sop, 1);
		(*p)++;
		semop(semid, &sov, 1);
	}
	printf("the value is %d\n", *p);
		
	return 0; 
}
