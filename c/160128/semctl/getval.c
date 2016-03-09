#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/types.h>
int main()
{
	int semid;
	semid=semget((key_t)1111, 1, 0600 | IPC_CREAT);
	printf("%d\n",	semctl(semid, 0, GETVAL));
	return 0;	
	
}
