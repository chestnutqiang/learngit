#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_ARG 20
#define MAX_LENGTH 64 
/* 宏，将cline串中从pos所表示的位置开始，跳过连续的空格和制表符 */
#define del_blank(pos, cline); { \
	while(cline[pos] != '\0' && (cline[pos] == ' ' || cline[pos] == '\t'))\
	{\
		pos++;\
	}\
}

/**/ 
#define get_arg(arg, pos, cline);{\
	int j = 0;\
	while(cline[pos] != '\0' && cline[pos] != ' ' && cline[pos] != '\t'){\
		arg[j++] = cline[pos++];\
	}\
}
typedef struct command_struct
{
	char *name;
	char *argv[MAX_ARG];
}command_line;
int split(command_line * command, char cline[])
{
	cline[strlen(cline) - 1] = '\0';
	int i;
	int pos = 0;
	del_blank(pos, cline);
	i = 0;
	while( cline[pos] != '\0')
	{
		if((command->argv[i] = (char*)calloc(MAX_LENGTH, sizeof(char))) == NULL)
		{
			perror("fail to calloc");
			exit(-1);
		}
		get_arg(command->argv[i], pos, cline);
		i++;
		del_blank(pos, cline);
	}
	command->argv[i] == NULL;
	command->name = command->argv[0];
}

int main()
{
	char line[256];
	//char line[100];
	command_line command;
	int i;
	memset(line, 0, sizeof(line));
	while(fgets(line, sizeof(line), stdin) != NULL)
	{
		i = 0;
		split(&command, line);
		while(command.argv[i] != NULL)
		{
			printf("%s\n", command.argv[i]);
			i++;
		}
		memset(line, 0, sizeof(line));
		i = 0;
		while (command.argv[i] != NULL)
		{
			free(command.argv[i]);
			i++;
		}
	}
}
