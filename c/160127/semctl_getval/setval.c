#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
int main()
{
	int semid,ret;
	semid = semget( (key_t)1234, 1, 0600 | IPC_CREAT);
	if(-1 == semid)
	{
		perror("semget");
		return -1;
	}
	union semun arg;	
	arg.val = 1;	
	ret = semctl(semid, 0, SETVAL, arg);
	if(-1 == ret)
	{
		perror("semctl");
		return -1;
	}
//	arg.val = 0;
	 ret = semctl( semid, 0, GETVAL,arg);
	if(-1 == ret )
	{
		perror("semctl");
		return -1;
	}
//	printf("arg.val is %d\n", arg.val);
	printf("ret is %d \n", ret);
	return 0;
}
