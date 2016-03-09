#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

//目的1：实现pipe1_2,pipe2_2之间的管道通信

int main(int argc, char* argv[]){
		if(argc < 3){
				printf("args are not enough.\n");
				return -1;
		}
		int frd, fwt, ret;
		frd = open(argv[1],O_RDONLY);
		fwt = open(argv[2],O_WRONLY);
		if((-1 == frd) || (-1 == fwt)){
				perror("open");
				return -1;
		}
		printf("管道建立成功.\n");
		fd_set readset;

		char buf[128];
		while(1){
				FD_ZERO(&readset);
				FD_SET(frd,&readset);
				FD_SET(STDIN_FILENO,&readset);
				ret = select(frd+1,&readset,NULL,NULL,NULL);
				if(-1 == ret){
						perror("select");
						return -1;
				}
				if(FD_ISSET(frd,&readset)){
						bzero(buf,sizeof(buf));
						ret = read(frd,buf,sizeof(buf));
						if(ret > 0)
								write(STDOUT_FILENO,buf,strlen(buf));
				}
				if(FD_ISSET(STDIN_FILENO,&readset)){
						bzero(buf,sizeof(buf));
						read(STDIN_FILENO,buf,sizeof(buf));
						write(fwt,buf,strlen(buf));
				}
		}
		return 0;
}

