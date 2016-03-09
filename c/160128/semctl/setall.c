#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/types.h>
int main()
{
	int semid;
	int ret;
	 semid=semget((key_t)1111,3, 0600 | IPC_CREAT);
	if(-1 == semid)
	{
		perror("semget");
		return -1;
	}
	struct semid_ds buf;
	unsigned short arr[3];
	ret = semctl(semid,0, GETALL, arr);	
	if(ret == -1)
	{
		perror("semctl");
		return -1;
	}
	printf("sem1=%d, sem2=%d, sem3=%d", arr[0], arr[1],arr[2]);
	arr[0] = 2;
	arr[1] = 3;
	arr[2] = 4;	
	ret = semctl(semid, 0, SETALL, arr);
	if(ret == -1)
	{
		perror("semctl");
		return -1;
	}	
	printf("sem1=%d, sem2=%d, sem3=%d", arr[0], arr[1],arr[2]);
	printf("%d\n",	semctl(semid, 0, GETVAL));
	return 0;	
	
}
