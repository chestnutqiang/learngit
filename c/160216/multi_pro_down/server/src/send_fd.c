#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<strings.h>
#include<stdlib.h>
#include<stdio.h>
void send_fd(int sfd, int fd)
{
	struct msghdr msg;
	bzero(&msg, sizeof(struct msghdr));
	struct iovec msg_iov[2];
	char buf1[10]={"hi,hello"};
	char buf2[10]={"hi,byebye"};
	msg_iov[0].iov_base = buf1;
	msg_iov[0].iov_len = 8;
	msg_iov[1].iov_base = buf2;
	msg_iov[1].iov_len = 9;
	msg.msg_iov = msg_iov;
	msg.msg_iovlen = 2;
	struct cmsghdr *cmsg; //必须定义成指针，因为一会要强制转成void*
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int *fdptr;
	fdptr = (int*)CMSG_DATA(cmsg);
	*fdptr = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;  //  msg_control是void*,不知道这个的偏移量
	int ret = sendmsg(sfd,&msg, 0);
	if(-1 == ret)
	{
		perror("sendmsg");
		exit(-1);
	}
}

/*
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		perror("args");
		return -1;
	}
	int ret;
	int sfd[2];
	ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
	if(-1 == ret)
	{
		perror("socketpair");
		return -1;
	}
	if(!fork())//子进程
	{
		close(sfd[1]);   //关闭描述符4
		int fd;
		recv_fd(sfd[0], &fd);
		printf("I am child process, fd is %d\n", fd); 
										//最小的描述符是上面关闭的4
		char buf[100];
		bzero(buf, sizeof(buf));
		read(fd, buf, sizeof(buf));
		printf("buf from fd reading is %s\n", buf);
	}
	else
	{
		close(sfd[0]);    //关闭描述符3
		int fd;
		fd = open(argv[1], O_RDWR);
		if(-1 == fd)
		{
			perror("open");
			exit(-1);
		}
		//最小的描述符是上面关闭的3
		printf("I am man process, fd is %d\n", fd);
		send_fd(sfd[1], fd);  //描述符4被sfd[1]占用
		wait(NULL);
	}
}
*/
