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
#define MAX_CMD 256		//命令行的最大长度
#define MAX_LEN 64   //每个参数或者命令的最大长度
#define MAX_ARG 10		//一个命令最大参数个数
#define PORT 8888
#define MAX_FILENAME_LEN 256  //文件名的最大长度

typedef struct cmd_struct	//命令行的结构类型
{
	char *name;
	char *argv[MAX_ARG];  
}cmd_t;
/*分离命令行的字符串*/
extern int split(cmd_t *command, char cline[]);
/*处理客户端的连接*/
extern int do_connect(char *ip, struct sockaddr_in *sin, int *sock_fd);
/*处理get命令，从服务器下载文件到*/
extern int do_get(const char *path,int sock_fd);
/*处理put命令，从本地上传文件到服务器*/
extern int do_put(const char *src, const char *dst, int sock_fd);
/*处理ls命令，列出服务器当前的文件*/
extern int do_ls(char *path, int sock_fd);
/*处理cd命令，进入对应目录*/
extern int do_cd(char *path, int sock_fd);
/*显示当前目录*/
extern int do_pwd(int sock_fd);
/*删除对应文件*/
extern int do_rm(char *path, int sock_fd);
/*客户端退出*/
extern int do_quit(int sock_fd);
#endif
