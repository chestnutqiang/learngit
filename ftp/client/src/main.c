#include"head.h"

int main(void)
{
	cmd_t cmd_line;
	char cline[256];
	char buf[1024];
	int sock_fd;
	struct sockaddr_in sin;
	bzero(&sin, sizeof(struct sockaddr));
	printf("myftp> ");
	fflush(stdout);
	bzero(&cmd_line, sizeof(cmd_line));
	while(fgets(cline, sizeof(cline), stdin) != NULL)
	{
		/*把字符串分离成命令和参数，存在cmd_line*/
		if(split(&cmd_line, cline) == -1)
			exit(1);

		//	printf("cmd_line.name = %s\n", cmd_line.name);//----------------------------------------

		if(strcasecmp(cmd_line.name, "get") == 0)
		{
			if( (do_get(cmd_line.argv[1],sock_fd)) == -1)
			{
				continue;
			}
			//		printf("exit do_get\n");
		}
		else if(strcasecmp(cmd_line.name, "put") == 0)
		{
			if( (do_put(cmd_line.argv[1], cmd_line.argv[1], sock_fd)) == -1)
			{
				continue;
			}
		}
		else if(strcasecmp(cmd_line.name, "ls") == 0)
		{
			//printf("0\n"); //-------------------------------------
			//	if(cmd_line.argv[1] == NULL)
			//	{
			if(do_ls("./", sock_fd) == -1)
			{
				continue;
			}
			//	}
			//	else
			//	{
			//		printf("cmd_line.argv[1] = %s\n", cmd_line.argv[1]);
			//		if(do_ls(cmd_line.argv[1], sock_fd) == -1)
			//			exit(1);
			//	}
			//	if(cmd_line.argv[2] != NULL)
			//	{
			//		goto err;
			//	}
		}
		else if(strcasecmp(cmd_line.name, "cd") == 0 )
		{
			//	if(cmd_line.argv[1] != NULL)
			//	{
			//		goto err;
			//	}
			if( do_cd(cmd_line.argv[1], sock_fd) == -1)
			{
				goto err;
			}
		}
		else if(strcasecmp(cmd_line.name, "pwd") == 0)
		{
			if( do_pwd(sock_fd) == -1)
				goto err;
		}
		else if(strcasecmp(cmd_line.name, "rm") == 0)
		{
			if( do_rm(cmd_line.argv[1], sock_fd) == -1)
				goto err;
		}
		else if(strcasecmp(cmd_line.name, "quit") == 0)
		{
			if( do_quit(sock_fd) == -1)
				exit(1);
			exit(0);
		}
		else if(strcasecmp(cmd_line.name, "connect") == 0)
		{
		/*	bzero(buf, sizeof(buf));
			recv(sock_fd, buf, sizeof(buf), 0);
			printf("%s\n", buf);
			*/
			if( do_connect(cmd_line.argv[1],&sin, &sock_fd ) == -1)
				goto err;
		}
		else 
		{
err:
			printf("wrong command\n");
			printf("usage: command argv1 argv2 ...\n");
		}
		/*使用完的参数要释放*/
		int III= 0;
		while (cmd_line.argv[III] != NULL)
		{
			free(cmd_line.argv[III]);
			cmd_line.argv[III] = NULL;
			III++;
		}
		/*
		   int III = 0;
		   while( cmd_line.argv[III] != NULL)
		   {
		   printf("%s\n", cmd_line.argv[III]);
		   III++;
		   }
		   */
		printf("myftp> ");
		fflush(stdout);
		memset(cline, 0, sizeof(cline));
	}
	if( close(sock_fd) == -1)
	{
		perror("fail to sock_fd");
		exit(1);
	}
	return 0;
}
