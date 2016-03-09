#include"head.h"

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
int split(cmd_t * command, char cline[])
{
	cline[strlen(cline) - 1] = '\0';
	int i;
	int pos = 0;
	del_blank(pos, cline);
	i = 0;
	while( cline[pos] != '\0')
	{
		if((command->argv[i] = (char*)calloc(MAX_LEN, sizeof(char))) == NULL)
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
