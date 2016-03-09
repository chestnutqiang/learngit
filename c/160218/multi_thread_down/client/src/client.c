#include"head.h"
int tcp_connect(const char *ip, int port)
{
	int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = inet_addr(ip);
	ret = connect(sfd, (struct sockaddr*)&my_addr,  sizeof(struct sockaddr));
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	return sfd;
}
int main(int argc, char *argv[])
{
	int sfd;
	sfd=tcp_connect("192.168.168.130", atoi("3000")); //ip和port
	printf("connect success!\n");
	data_t buf;
	bzero(&buf, sizeof(buf));          
	int ret;
	printf("1\n");
	ret = recv(sfd,&buf.len,sizeof(int), 0);    //接收文件名字的长度
	if(-1 == ret)
	{
		perror("recv: receive filename length");
		exit(-1);
	}
	printf("2\n");
	ret = recv(sfd,buf.t_data,buf.len,0);			//接收文件名
	if(-1 == ret)
	{
		perror("recv:recvive filename");
		exit(-1);
	}
	int fd;
	fd = open(buf.t_data, O_RDWR | O_CREAT, 0666);  //创建一个同名文件
	if(-1 == fd)
	{
		perror("open");
		exit(-1);
	}
	int flag = 0;   //用来标记文件传输完毕
	printf("3\n");
	while(1)
	{
		bzero(&buf, sizeof(buf));
		ret = recv(sfd,&buf,sizeof(int), 0);   //从服务端接收文件数据的长度
		if(-1 == ret)
		{
			perror("recv: sfd");
			exit(-1);
		}
		printf("4\n");
		recv_n(sfd,buf.t_data,buf.len);      //从服务端接收文件数据，循环接收数据
	printf("5\n");
		if(buf.len == sizeof(int) && !memcmp(buf.t_data, &flag, 4))   //判断文件接收完毕
		{
			break;
		}
	//	printf("-----------%s\n", buf.t_data);
		ret = write(fd, buf.t_data, buf.len);//写入到client的文件中
		if(-1 == ret)
		{
			perror("write");
			close(fd);
			return -1;
		}
	}
	printf("file download successful!\n");
	close(fd);
}

