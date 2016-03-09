#include"head.h"

int main()
{
	int pid, new_fd, lfd, sock_opt, ret;
	char buf[MAX_LEN];
	struct sockaddr_in sin;
	sock_opt = 1;
	if(sock_init(&sin, &lfd, sock_opt) == -1)
	{
		perror("fail to sock_init");
		exit(1);
	}
	//epoll
	while(1)
	{
		//epoll_wait();		
		if( (new_fd = accept(lfd, NULL, NULL)) == -1)
		{
			perror("fail to accept");
			exit(1);
		}
		printf("connect successful\n");
		//send(new_fd, "connect successful\n", strlen("connect successful\n"), 0);
		if( (pid = fork()) < 0) //出错处理
		{
			perror("fail to fork");
			exit(1);
		}
		else if( pid == 0) //子进程处理
		{
			close(lfd);
			while(1)
			{
				//printf("next cmd\n");    //----------------------
				memset(buf, 0, sizeof(buf));
				if((ret = recv(new_fd, buf, sizeof(buf), 0)) == -1) //接受客户端的命令
				{
					perror("fail to accept");
					exit(1);
				}
				else if(ret == 0)
				{
					printf("There is a client eixt\n");
					exit(1);
				}
				//printf("accept data from  new_fd\n");   //-----------------------
				if(strstr(buf, "CD") == buf)  //返回的函数指针正好等于buf
				{
					//	printf("entry CD\n");
					do_cd(&buf[3], new_fd);
					//	printf("exit CD\n");
				}
				else if(strstr(buf, "GET") == buf)
				{
					//	printf("entry GET\n");
					do_put(&buf[4], new_fd);
					//	printf("exit GET\n");
				}
				else if(strstr(buf,"LS") == buf)
				{
					//	printf("enter LS\n");
					do_ls(&buf[3], new_fd);
					//	printf("exit LS\n");
				}
				else if(strstr(buf,"PUT") == buf)
				{//传 path, file_len,new_fd 文件长度是8字节
					char buf1[9];  //buf1存文件长度
					bzero(buf1, sizeof(buf1));
					int space1,space2;
					space1 = strstr(buf, " ") - buf; //第一个空格的位置
					space2 = strstr(buf+4," ") - buf; //第二个空格的位置
					memcpy(buf1, buf+4,space2-space1-1); //两个空格中间是传送文件的长度
					//printf("buf=%s,buf1=%s\n", buf,buf1); //-----------------------
					do_get(&buf[space2+1],atoi(buf1), new_fd);
					//	printf("exit PUT"); //-------------------
				}
				else if(strstr(buf, "QUIT") == buf)
				{
					continue;  //客户端那边退出，recv会在下一次从new_fd接受数据后返回
				}
				else if(strstr(buf, "PWD") == buf)
				{
					if(do_pwd(new_fd) == -1)
						exit(1);
				}
				else if(strstr(buf, "RM") == buf)
				{
					if(do_rm(&buf[3],new_fd) == -1)
						exit(1);
				}
				else
				{
				//	printf("any times receive bytes number = %d\n", ret);
					continue;
				}
			}
			close(new_fd);
			exit(0);
		}
		else  //父进程处理
		{
			close(new_fd);
		}
	}
	return 0;
}
