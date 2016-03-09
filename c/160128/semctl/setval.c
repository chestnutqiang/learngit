#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/types.h>
int main()
{
	int semid;
	semid=semget((key_t)1111, 1, 0600 | IPC_CREAT);
	struct semid_ds buf;
	semctl(semid,0,IPC_STAT, &buf);
	printf("uid = %d, gid = %d, cuid = %d, cgid = %d\n", buf.sem_perm.uid, buf.sem_perm.gid, buf.sem_perm.cuid, buf.sem_perm.cgid);
	buf.sem_perm.mode = 0640;
	semctl(semid,0,IPC_SET, &buf);
	printf("uid = %d, gid = %d, cuid = %d, cgid = %d\n", buf.sem_perm.uid, buf.sem_perm.gid, buf.sem_perm.cuid, buf.sem_perm.cgid);

	printf("%d\n",	semctl(semid, 0, GETVAL));
	return 0;	
	
}
