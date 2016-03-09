#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

int main()
{
	int semid;
	semid = semget( (key_t)1234, 1, 0600 | IPC_CREAT);
	int ret = semctl( semid, 0, GETVAL);
	printf("ret is %d\n", GETVAL);
	return 0;
}
