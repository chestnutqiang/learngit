#include"head.h"


/*处理客户端的连接*/
int do_connect(char *ip, struct sockaddr_in *sin, int *sock_fd)
{
	int sfd;
	if( (sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("fail to create sock");
		return -1;
	}
	bzero(sin, sizeof(struct sockaddr));
	sin->sin_family = AF_INET;
	sin->sin_port = htons(PORT);
	sin->sin_addr.s_addr = inet_addr(ip);
	if( (connect(sfd, (struct sockaddr*)sin, sizeof(struct sockaddr))) == -1)
	{
		perror("fail to connect");
		return -1;
	}
	*sock_fd = sfd;
	return 0;
}
/*处理get命令，从服务器下载文件到本地*/
int do_get(const char *path, int sock_fd)
{
//printf("path = %s,  sock_fd = %d\n", path, sock_fd);  //---------------------------------
	int n;
	int fd;
	char buf[1024];
	int res = -1;
	long len;
	bzero(buf, sizeof(buf));
	sprintf(buf, "GET ./%s", path);
	if(send(sock_fd, buf, strlen(buf), 0) == -1)
	{
		perror("send GET fail");
		return -1;
	}
	bzero(buf, sizeof(buf));
	if(recv(sock_fd, buf, sizeof(buf), 0) == -1) //文件长度
	{
		perror("recv file length fail");
		return -1;
	}
	if(buf[0] == 'E')
	{
		printf("%s\n", buf+4);
		return -1;
	}
	len = atoi(&buf[3]);	
	printf("2 len = %ld\n", len);
	bzero(buf, sizeof(buf));
	sprintf(buf, "./%s",path);
	fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if(fd == -1)
	{
		perror("fail to open file");
		return -1;
	}
	/*循环接受文件*/
	while(1)
	{
		bzero(buf, sizeof(buf));
		n = recv(sock_fd,buf, sizeof(buf), 0);
	//	printf("2, %d\n", n);		//----------------------------------
		if(n > 0)
		{
			if( write(fd, buf, n) == -1)
			{
				perror("write");
				return -1;
			}	
			write(STDOUT_FILENO, buf, n);
			len -= n;
			if(len == 0)
			{
				break;
			}
		}
		else 
		{
			exit(1);			
		}
	}
	return 0;
}
/*处理put命令，从本地上传文件到服务器*/
int do_put(const char *src, const char *dst, int sock_fd)
{
	//printf("0\n"); //-------------------------------
	struct stat statbuf;
	int n, fd;
	int res = -1;
	char buf[1024];
	if(src == NULL)
	{
		printf("No such file or directory\n");
		return -1;
	}
	if(src[strlen(src)-1] == '/')
	{
		printf("No such file or directory\n");
		return -1;
	}
	char srcpath[256];
	bzero(srcpath, sizeof(srcpath));
	sprintf(srcpath, "./%s", src);
	fd = open(srcpath, O_RDONLY, 0664);
	if( -1 == fd)
	{
		printf("No such file or directory\n");
		return -1;
	}
	if(fstat(fd, &statbuf) == -1)
	{
		perror("fail to get file status");
		goto end2;
	}
	//printf("1\n"); //------------------------------------

	sprintf(buf,"PUT %ld ./%s", statbuf.st_size, dst); //64位系统ld是8字节sizeof()
	if(send(sock_fd, buf, strlen(buf), 0) == -1 )  //sizeof(long int) 是 ld型
	{
		perror("fail to send PUT");
		goto end2;
	}
	//printf("2 %s \n", buf); //---------------------------------
	memset(buf, 0, sizeof(buf));
	if( recv(sock_fd, buf, sizeof(buf), 0) == -1) //确认信息
	{
		perror("fail to receive answer");
		goto end2;
	}
	if(buf[0] == 'E')
	{
		perror("server can't receive");
		goto end2;
	}
	/*客户端循环读文件发给服务端*/
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		n = read(fd, buf,sizeof(buf));
		//printf("3, %d\n", n); //-----------------------------------------
	//	write(STDOUT_FILENO, buf, n);
		if(n > 0)
		{		
			if(send(sock_fd, buf, n, 0) == -1)
			{
				goto end2;
			}
		}
		else if(n == 0)
		{
			printf("OK\n");
			break;	
		}
		else
		{
			perror("fail to read");	
			goto end2;
		}
			 
	}
	res = 0;
end1:
	close(fd);
end2:
	return res;	
}
/*处理ls命令，列出服务器当前的文件*/
int do_ls(char *path, int sock_fd)
{
	char buf[MAX_LEN];
	int len;
	int n;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "LS %s", path);
	if ( (send(sock_fd, buf, strlen(buf)+1, 0)) == -1) /*向服务器发送LS命令*/
	{
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	//printf("1\n"); //--------------------------------------------------
	if(( n = recv(sock_fd, buf, sizeof(buf), 0)) == -1) /*接受服务器的应答码，含有服务器传来的文件长度*/ 
	{
		return -1;
	}
	if(buf[0] == 'E')  /*服务器发来出错信息*/
	{
		printf("%s\n", buf);
		return 0;
	}
	len = atoi(&buf[3]);   /*跳过OK和空格，是要传输的文件长度*/  
	if(send(sock_fd,"RDY", 3, 0 ) == -1 ) /*通知服务器准备接受文件*/
	{
		return -1;
	}
	//printf("2\n"); //----------------------------------------
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		n = recv(sock_fd, buf, sizeof(buf), 0);
		//printf("3\n"); //--------------------------
		if(n > 0)
		{
			write(STDOUT_FILENO, buf, n);
			len -= n;
			//printf(" n = %d, len = %d\n", n, len);
			if(len == 0) break;
		}
		else if(len == 0)
		{
			break;
		}
		else if(n < 0)  //读取出错
		{
			return -1;
		}

	}

	//printf("4\n" ); //---------------------------------
}
/*处理cd命令，进入对应目录*/
int do_cd(char *path, int sock_fd)
{
	//printf("0\n");  //--------------------------------
	char buf[MAX_CMD];
	bzero(buf, sizeof(buf));
	sprintf(buf, "CD %s", path);
	if(send(sock_fd, buf, sizeof(buf), 0) == -1)
	{
		perror("fail to send");
		return -1;
	}
	//printf("1\n");  //--------------------------------
	bzero(buf, sizeof(buf));
	if(recv(sock_fd, buf, sizeof(buf), 0) == -1)
	{
		perror("fail to recv");
		return -1;
	}
	//printf("2\n");  //--------------------------------
	if( buf[0] == 'E')
	{
		write(STDOUT_FILENO, buf+4, strlen(buf)-4); //不输出头部的ERR和空格
	}
	return 0;
}
int do_pwd(int sock_fd)
{
	if(send(sock_fd, "PWD", strlen("PWD"), 0) == -1)
	{
		return -1;
	}
	char buf[MAX_LEN];
	bzero(buf, sizeof(buf));
	if(recv(sock_fd, buf, sizeof(buf), 0) == -1)
	{
		return -1;
	}
	printf("%s\n", buf);
	return 0;
}
int do_rm(char *path, int sock_fd)
{
	char buf[1024];
	bzero(buf,sizeof(buf));
	sprintf(buf, "RM %s", path);
	if(send(sock_fd, buf, sizeof(buf), 0) == -1)
	{
		perror("fail to send");
		return -1;
	}
	bzero(buf, sizeof(buf));
	if(recv(sock_fd, buf, sizeof(buf), 0) == -1)
	{
		perror("fail to recv");
		return -1;
	}
	if(buf[0] == 'E')
	{
		printf("%s\n", buf+4);
		return 0;
	}
	return 0;
}
/*客户端退出*/
int do_quit(int sock_fd)
{
	char buf[MAX_LEN];
	memset(buf, 0,sizeof(buf));
	sprintf(buf, "QUIT");
	if( (send(sock_fd, buf, strlen(buf)+1, 0)) == -1)
		return -1;
	return 0;
}
