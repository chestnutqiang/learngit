#include"head.h"
void recv_fd(int sfd, int*fd)
{
	struct msghdr msg;
	bzero(&msg, sizeof(struct msghdr));
	struct iovec msg_iov[2];
	char buf1[10]={0};
	char buf2[10]={0};
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
	msg.msg_control = cmsg;
	msg.msg_controllen = len;  //  msg_control是void*,不知道这个的偏移量
	int ret = recvmsg(sfd, &msg, 0);
	if(-1 == ret)
	{
		perror("recvmsg");
		exit(-1);
	}
	*fd = *(int*)CMSG_DATA(cmsg);
	//printf("The fd of recv is %d\n", *fd);
}
