#include"head.h"

void make_child(pchild child_ptr, int n)
{
	int pid;
	int ret;
	int i;
	int fd[2];
	for(i = 0; i < n; i++)
	{
		ret = socketpair(AF_LOCAL, SOCK_STREAM, 0,fd);
		if(-1 == ret)
		{
			perror("socketpair");
			exit(-1);
		}
		pid = fork();
		if(pid == 0)
		{
			close(fd[1]);
			child_handle(fd[0]);
		}
		close(fd[0]);
		child_ptr[i].pid = pid;
		child_ptr[i].sfd=fd[1];
		child_ptr[i].busy = 0;
	}
}
void child_handle(int sfd)
{
	int fd;
	int flag = 1;
	while(1)
	{
		recv_fd(sfd, &fd);   //fd 就是new_fd
		send_file(fd);
		write(sfd,&flag, 4);
	}
}

void send_file(int send_file_fd)
{
	//先传送文件的名字
	int ret;
	data_t buf;
	buf.len=strlen(DOWN_FILE);				//data_t == data_type  t_data == transmission_data
//	memcpy(buf.t_data, DOWN_FILE, buf.len); //t_data是传输的内容 
	strcpy(buf.t_data, DOWN_FILE);
	ret = send(send_file_fd, &buf, buf.len+4, 0);
	if(-1 == ret)
	{
		perror("send filename");
		exit(-1);
	}
	//打开文件	
	int fd;
	fd = open(DOWN_FILE, O_RDONLY);
	//循环读文件，然后发送给客户端
	if(-1 == fd)
	{
		perror("open");
		return ;
	}
	while(1)
	{
		bzero(&buf, sizeof(buf));
		buf.len=read(fd, buf.t_data, sizeof(buf.t_data));  //读取文件内容   //出现严重错误 调bug一天 将=写成==
		//printf("read %d : %s\n",buf.len, buf.t_data);
		if(buf.len <= 0)
		{
			break;
		}
		send_n(send_file_fd, (char*)&buf, buf.len+4);
		//printf("ret = %d\n", ret);
	}
	//通知客户端发送数据结束
	bzero(&buf, sizeof(buf));
	int flag = 0;
	buf.len=sizeof(int);
	memcpy(buf.t_data, &flag, buf.len);
	send(send_file_fd, &buf, buf.len+4, 0);
	close(fd);  //关闭打开文件的文件描述符 
	close(send_file_fd);   //关闭连接客户端的文件描述符
}
