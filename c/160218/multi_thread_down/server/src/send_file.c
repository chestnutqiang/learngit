#include"factory.h"
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
