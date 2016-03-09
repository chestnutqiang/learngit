#include"head.h"

int sock_init(struct sockaddr_in *sin, int *lfd, int sock_opt)
{
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("fail to create sfd");
		return -1;
	}
	/*设置允许重用本地地址和端口*/
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int));
	bzero(sin, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_port = htons(PORT);
	sin->sin_addr.s_addr = inet_addr(IP);
	if( bind(sfd,(struct sockaddr*)sin, sizeof(struct sockaddr) ) == -1)
	{
		perror("fail to bind");
		return -1;
	}
	if( listen(sfd, MAX_CONNECT) == -1)
	{
		perror("fail to listen");
		return -1;
	}
	*lfd = sfd;
	return 0;
}
/*接受的文件在服务器端的存储路径，文件的长度，服务端和客户端通信的描述符*/
int do_get(const char *path, long len, int sock_fd)
{
//	printf("path = %s, len = %ld, sock_fd = %d\n", path, len, sock_fd); //-------------------
	int n;
	int fd;
	char buf[1024];
	int res = -1;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if( -1 == fd)
	{
		if(errno == EISDIR)
		{
			if(send(sock_fd, "ERR", strlen("ERR"), 0) == -1)
			{
				perror("send");
				goto end;
			}
			res = 0;
			goto end;
		}
		else
		{
			if(send(sock_fd, "ERR server fail to open file", 
						strlen("ERR server fail to oepn file"), 0) == -1)
			{
				perror("send");
				goto end;
			}
		}
	}
	else
	{
		if(send(sock_fd, "OK", strlen("OK"), 0) == -1)
		{
			perror("send");
			return -1;
		}
	}
	//printf("1\n"); //---------------------------------------------
	/*循环接受文件*/
	while(1)
	{
		bzero(buf, sizeof(buf));
		n = recv(sock_fd,buf, sizeof(buf), 0);
		//printf("2, %d\n", n); //--------------------------------
		if(n > 0)
		{
			if( write(fd, buf, n) == -1)
			{
				perror("write");
				return -1;
			}
			len -= n;
			if(len == 0) 
			{
				printf("Receive file complete\n");
				break;
			}
		}
		else
		{
			goto end;
		}
	}
	//printf("2\n"); //---------------------------------------------------
	res = 0;
end:
	close(fd);
	return res;
}
int do_put(const char *path, int sock_fd)
{
	//printf("0\n"); //-------------------------------
	struct stat statbuf;
	int n, fd;
	int res = -1;
	char buf[1024];
	int len;
	char msg_to_client[] = "ERR No such file or directory\n"; //发送给客户端的消息
	if(path == NULL)
	{
		send(sock_fd, msg_to_client, strlen(msg_to_client), 0);
		return -1;
	}
	if(path[strlen(path)-1] == '/')
	{
		send(sock_fd, msg_to_client, strlen(msg_to_client), 0);
		return -1;
	}
	char srcpath[256];
	bzero(srcpath, sizeof(srcpath));
	sprintf(srcpath, "./%s", path); //当前目录下的文件
	fd = open(srcpath, O_RDONLY, 0664);
	if( -1 == fd)
	{
		send(sock_fd, msg_to_client, strlen(msg_to_client), 0);
		return -1;
	}
	if(fstat(fd, &statbuf) == -1)
	{
		perror("fail to get file status");
		goto end;
	}
	//printf("1\n"); //------------------------------------
	bzero(buf, sizeof(buf));
	sprintf(buf,"OK %ld", statbuf.st_size); //64位系统ld是8字节sizeof()
	if(send(sock_fd, buf, strlen(buf), 0) == -1 )  //sizeof(long int) 是 ld型
	{
		send(sock_fd, "ERR get file fail", strlen("ERR get file fail"), 0);
	}
	//printf("2 %s \n", buf); //---------------------------------
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		n = read(fd, buf, sizeof(buf));
		if(n > 0)
		{
			if(send(sock_fd, buf, n, 0) == -1)
			{
				perror("send fail");
				return -1;
			}
		}
		else
		{
			goto end;
		}
	}
	/*客户端循环读文件发给服务端*/
end:
	close(fd);
	return 0;
}
/* 手动system(ls) 的代码 */
#define p1 0x0007
#define p2 0x0038
#define p3 0x01c0
char* substr(char s[])
{
	char *ss;
	ss = s+4;
	ss[12] = '\0';
	return ss;			
}
char Isrwx(unsigned short n)
{
	if( (n&S_IRUSR) || (n&S_IRGRP) || (n&S_IROTH))
	{
		return 'r';
	}else if((n&S_IWUSR) || (n&S_IWGRP) || (n&S_IWOTH)){
		return 'w';	
	}else if( (n&S_IXUSR) || (n&S_IXGRP) || (n&S_IXOTH) ){
		return 'x';
	}
	else{
		return '-';
	}
}
void  aut(unsigned short num, char buf[])
{
	char filetype;
	if(num & S_IFREG){
		filetype = '-';
	}else{
		filetype = 'd';
	}
	sprintf(buf, "%c%c%c%c%c%c%c%c%c%c", filetype,Isrwx(num&S_IRUSR),Isrwx(num&S_IWUSR),Isrwx(num&S_IXUSR),Isrwx(num&S_IRGRP),Isrwx(num&S_IXGRP),Isrwx(num&S_IXGRP),Isrwx(num&S_IROTH),Isrwx(num&S_IWOTH),Isrwx(num&S_IXOTH));
}
/*以上是system(ls)的代码*/
int do_ls(char *path, int sock_fd)
{

	//printf("0\n"); // ------------------------------------------------
	int res = -1;
	//printf("chdir---%s\n", path);
	if( chdir(path) == -1)
	{
		perror("fail to chdir");
		return -1;
	}
	DIR *dir;
	dir = opendir(path);
	if(dir == NULL)
	{
		perror("fail to opendir");
		return res;
	}
	struct dirent * pdir;
	struct stat  st1;
	int ret;
	char buf[1024];
	/*要写入和读取的临时文件*/
	 FILE * fp;
	 fp = fopen(".temp", "w+");
	 if(fp == NULL)
	 {
		perror("fail to fopen");
		return res;
	 }
	while( pdir= readdir(dir))
	{
		if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
		{
			continue;
		}
		bzero(buf, sizeof(buf));
		sprintf(buf,"%s%s",path, pdir->d_name); 
		ret = stat( buf, &st1);	
		if(ret == -1)
		{
			perror("stat");
			return -1;
		}
		char buf1[50];
		aut(st1.st_mode, buf1);	
	//printf("%s\n", buf);  //-------------------------------
	/*	printf("%-11s%3ld%15s%15s%10ld%15s%10s\n",buf1,st1.st_nlink,getpwuid(st1.st_uid)->pw_name,
 getgrgid(st1.st_gid)->gr_name,st1.st_size,substr(ctime(&st1.st_mtime)), pdir->d_name);
	*/
	fprintf(fp, "%-11s%3ld%15s%15s%10ld%15s%10s\n",buf1,st1.st_nlink,getpwuid(st1.st_uid)->pw_name,
 getgrgid(st1.st_gid)->gr_name,st1.st_size,substr(ctime(&st1.st_mtime)), pdir->d_name);
	}
	fclose(fp);  //先关闭文件才能继续读取大小。
	//printf("1\n"); // ----------------------------------------------------
	/*打开临时文件，读取当前目录*/
	int fd;
	if( (fd = open(".temp", O_RDONLY)) == -1)
	{
		send(sock_fd, "ERR", strlen("ERR"), 0);
		return res;
	}
	/*得到文件状态*/
	struct stat statbuf;	
	if( fstat(fd, &statbuf) == -1)
	{
		perror("fstat");
		send(sock_fd, "ERR", strlen("ERR"), 0);
		goto end;
	}
	bzero(buf, sizeof(buf));
	sprintf(buf, "OK %ld", statbuf.st_size);
	//printf("3\n");	 //-------------------------------------
	if(send(sock_fd, buf, strlen(buf) + 1, 0) == -1) /*OK和文件长度*/
	{
		goto end;
	}
	//printf("4\n"); //---------------------------------------
	if( recv(sock_fd, buf, MAX_LEN, 0) <= 0 )
	{
		goto end;
	}
	//printf("5\n"); // -----------------------------------------
	/*循环读取temp，发送给客户端*/
	int n;
	while(1)
	{
		bzero(buf, sizeof(buf));
		n = read(fd, buf, sizeof(buf));
		//printf("n = %d\n", n);      //------------------------------------
        if(n == 0)break;
		//write(STDOUT_FILENO, buf, n); //--------------------------
		if(n > 0)
		{
			if( send(sock_fd, buf, n, 0) == -1)
			{
				goto end;
			}
		}
		else if(n == 0)
		{
			break;
		}
		else
		{
			perror("fail to read");
			goto end;
		}
	}
	//printf("6\n");  //---------------------------------
	res = 0;		//执行到这里说明一切执行正确
	if(remove(".temp") == -1)
	{
		perror("fail to rm temp");
	}
end:
	//printf("7\n"); //--------------------------------
	close(fd);
	return res;
}
int do_cd(char *path, int sock_fd)
{
	//printf("0 %s\n", path);  //--------------------------------
	char dir[1024];
	bzero(dir, sizeof(dir));
	sprintf(dir, "./%s", path);
	if( chdir(dir) == -1)
	{
		if( send(sock_fd, "ERR -bash: cd: l: No such file or directory\n", 
					strlen("ERR -bash: cd: l: No such file or directory\n"), 0) == -1)
		{
			perror("fail to send change directory");
			return -1;
		}
	}
	else if(send(sock_fd, "OK\n", strlen("OK\n"), 0) == -1)
	{
		perror("fail to send");
		return -1;
	}
	//printf("1\n");  //--------------------------------
	return 0;
}

int do_rm(char *path, int sock_fd)
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	sprintf(buf, "./%s", path);
	
	if( remove(buf) == -1)
	{
		bzero(buf, sizeof(buf));
		sprintf(buf,"ERR rm: cannot remove '%s' : No such file or directory",path);
		send(sock_fd, buf, strlen(buf), 0);
	}
	else
	{
		send(sock_fd, "OK", strlen("OK"), 0);
		return 0;
	}
}

int do_pwd(int sock_fd)
{	
	char buf[MAX_LEN];
	getcwd(buf, sizeof(buf));
	if(send(sock_fd, buf, strlen(buf), 0) == -1)
	{
		perror("fail to send");
		return -1;
	}
}
