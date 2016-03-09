#ifndef __HEAD__
#define __HEAD__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

#define MAX_CMD 256		//命令行的最大长度
#define MAX_LEN 64   //每个参数或者命令的最大长度
#define MAX_ARG 10		//一个命令最大参数个数
#define PORT 8888
#define MAX_FILENAME_LEN 256  //文件名的最大长度
#define IP "192.168.4.88"
#define MAX_CONNECT 20
typedef struct cmd_struct	//命令行的结构类型
{
	char *name;
	char *argv[MAX_ARG];  
}cmd_t;
extern int sock_init(struct sockaddr_in *sin, int *lfd, int sock_opt);
extern int do_get(const char *path, long len, int sock_fd);
extern int do_put(const char *path,int sock_fd);
extern int do_ls(char *path,int sock_fd);
extern int do_cd(char *path,int sock_fd);
extern int do_rm(char *path, int sock_fd);
extern int do_pwd(int sock_fd);
#endif
