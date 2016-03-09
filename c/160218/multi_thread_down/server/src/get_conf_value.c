#include"factory.h"
char* get_conf_value(char *filepath, char *valuename, char *res)
{
	memset(res, 0, strlen(res));
	FILE *fp;
	fp = fopen(filepath, "r");
	if(fp == NULL)
	{
		printf("filepath is not exit\n");
		return ;
	}
	char buf[1024];
	char temp[128];
	int i, j, len, equal_pos;
	memset(buf, 0, sizeof(buf));
	//while( (fgets(buf, sizeof(buf), fp)) != NULL)
	while( fscanf(fp,"%s", buf) != EOF)
	{
		len = strlen(buf);
		equal_pos = 0;    //等号位置等于零或者等于len，读取的是错误信息。
		memset(temp, 0, sizeof(temp));
		for(i = 0; i < len; i++)
		{
			if(buf[i] != '=')
			{
				temp[i] = buf[i];
			}
			else
			{
				temp[i] = '\0';
				equal_pos = i;
				break;
			}
		}
		if(i == len)
		{
			equal_pos = len;
		}
		if(equal_pos == len || equal_pos == 0)
		{
			printf("config information is not exit %s\n", valuename);
			exit(-1);
		}
		if(strcmp(temp, valuename) == 0) //找到了想要的值
		{
			strcpy(res, buf+equal_pos+1);  //取等号后面的值
			return res; //返回获取值指针
		}
		memset(buf, 0, sizeof(buf));
	}
}
