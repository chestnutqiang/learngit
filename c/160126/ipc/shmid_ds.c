#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#define PROJ_ID 1

int main(int argc, char *argv[])
{
	key_t skey = ftok(argv[1],PROJ_ID);
	if(-1 == skey)
	{
		perror("ftok");
		return -1;
	}
	int shmid;
	shmid = shmget(skey, 1 << 12, 0600 | IPC_CREAT);
	if(-1 == shmid)
	{
		perror("shmget");
		return -1;
	}	
	struct shmid_ds ds;
	int ret;
	ret = shmctl(shmid, IPC_STAT, &ds);
	printf("segsz=%d,atime=%d,dtime=%d,cpid = %d,lpid=%d,nattch=%ld \n",
	ds.shm_segsz, ds.shm_atime, ds.shm_dtime,ds.shm_cpid, ds.shm_lpid, ds.shm_nattch  );
		
	printf("uid=%d, gid=%d, cuid=%d, cgid=%d\n", 
	ds.shm_perm.uid, ds.shm_perm.gid, ds.shm_perm.cuid, ds.shm_perm.cgid);
	return 0;
}
