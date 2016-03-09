#include"char.h"
char buf[128];
int opp_pid;   //对方pid
void handler(int s)
{
	bzero(buf, sizeof(buf));
	strcpy(buf, "bye");	
}

int main(int argc, char *argv[])
{
	system("clear");
	int fdr, fdw;
	fdr = open(argv[1], O_RDONLY);
	fdw = open(argv[2], O_WRONLY);
	if( -1 == fdr || -1 == fdw)
	{
		perror("open");
		return -1;
	}
	int ret,shmid;
	fd_set set;
	system("clear");
	printf("建立链接成功\n");
	key_t key;
	key = ftok(argv[2], 1); //共享内存 信号量用到的key
	shmid = shmget(key, 1 << 10, 0640 |IPC_CREAT);
	if(-1 == shmid)
	{
		perror("shmget");
		return -1;
	}
	char *shmaddr;
	shmaddr = shmat(shmid, NULL, 0);
	bzero(shmaddr, 1024);
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
	semp.sem_num = 0;
	semp.sem_op = -1;
	semp.sem_flg = SEM_UNDO;
	bzero(&semv, sizeof(semv));
	semv.sem_num = 0;
	semv.sem_op =  1;
	semv.sem_flg = SEM_UNDO;
	

	//初始化信号量
	ret = semctl(semid,0,SETVAL, 1);
	if(-1 == ret)
	{
		perror("semctl");
		return -1;
	}
	//信号处理
	signal(SIGINT, handler);
	while(1)
	{
		if(strcmp(buf,"bye") == 0)
		{
			write(fdw, buf, 3);
			strcpy(shmaddr, "bye");
			ret = shmdt(shmaddr);
			if(ret == -1)
			{
				perror("shmdt");
				return -1;
			}
			ret=semctl(semid,IPC_RMID,0);
			if(-1 == ret)
			{
				perror("semctl");
				return -1;
			}
			break;
		}
		FD_ZERO(&set);
		FD_SET(fdr, &set);
		FD_SET(STDIN_FILENO, &set);
		if( select(fdr+1, &set,NULL, NULL, NULL) > 0 )
		{
			if( FD_ISSET(STDIN_FILENO, &set) )
			{
				if( (ret = read(STDIN_FILENO,buf, sizeof(buf))) > 0)
				{
					write(fdw, buf, ret-1); 
					bzero(buf, sizeof(buf));
				}
			}
			if( FD_ISSET(fdr, &set) )
			{
				if( (ret = read(fdr, buf, sizeof(buf))) > 0)
				{
				//	write(STDOUT_FILENO, buf, ret);   				
					semop(semid, &semp, 1);
				//	sprintf(shmaddr, "%s", buf);    //写到共享内存中
					
					if(strcmp(buf,"bye") == 0)
					{
						strcpy(shmaddr, "bye");
						shmdt(shmaddr);
						semctl(semid,IPC_RMID,0);
						if(-1 == ret)
						{
							perror("semctl");
							return -1;
						}
						exit(0);
					}
					strcpy(shmaddr, buf);
			//		printf("%s\n", shmaddr);
					bzero(buf, sizeof(buf));
					semop(semid, &semv, 1);
				}	
			}
		}
	}
}
