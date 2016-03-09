#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>
#include<string.h>
#include<unistd.h>
#define DOWN_FILE "test.avi"

typedef struct pro_child
{
	int pid;
	int sfd;
	int busy;
}child,*pchild;
typedef struct data_t
{
	int len;
	char t_data[1000];
}data_t;
void make_child(pchild, int);
void child_handle();
void recv_fd(int,int*);
void send_fd(int,int);
void send_file(int);
void send_n(int,char*,int);
