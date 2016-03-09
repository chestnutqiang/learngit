#include"char.h"

int main(int argc, char *argv[])
{
	system("clear");
	//共享内存映射
	key_t key;
	key = ftok(argv[1], 1);
	int shmid;
	shmid = shmget(key, 1 << 10, 0640| IPC_CREAT);
	if(-1 == shmid)
	{
		perror("shmget");
		return -1;
	}
	char *shmaddr;
	shmaddr = shmat(shmid, NULL, 0);
	if((char*)-1 == shmaddr)
	{
		perror("shmat");
		return -1;
	}
	//信号量设置
	int semid;
	semid = semget(key, 2, 0640 | IPC_CREAT );
	if( -1 == semid)
	{
		perror("semid");
		return -1;
	}
	//初始化 p v 操作
	struct sembuf semp;    // p->  -1
	struct sembuf semv;    // p->  +1
	bzero(&semp, sizeof(semp));
	bzero(&semv, sizeof(semv));
	semp.sem_num = 0;
	semp.sem_op = -1;
	semp.sem_flg = SEM_UNDO;
	semv.sem_num = 0;
	semv.sem_op =  1;
	semv.sem_flg = SEM_UNDO;
	
	int ret;
	//初始化信号量
	ret = semctl(semid,0,SETVAL, 1);
	if(-1 == ret)
	{
		perror("semctl");
		return -1;
	}
	while(1)
	{
		if(shmaddr[0] != 0)
		{
		semop(semid, &semp, 1);    //p操作
			//读取操作操作
		if( strcmp(shmaddr, "bye") == 0)
		{
			ret=shmdt(shmaddr);
			if(-1 == ret)
			{
				perror("shmdt");
				return -1;
			}
			ret=shmctl(shmid,IPC_RMID,0);
			if(-1 == ret)
			{
				perror("shmctl");
				return -1;
			}
			exit(0);
		}
			printf("%s\n", shmaddr);	
			bzero(shmaddr,1024);
		semop(semid, &semv, 1); 	//v操作
		}
	}
	return 0;
}
