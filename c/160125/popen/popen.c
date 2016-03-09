#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	FILE *fp = popen("./scanf","w");
	if(fp == NULL)
	{
		perror("popen");
		return -1;
	}
	char buf[100];
	strcpy(buf, "jinxiaohui");
	int ret;
	ret = fwrite(buf,sizeof(char),strlen(buf),fp);
	if(ret < 0)
	{
		perror("fwrite");
		return -1;
	}
	pclose(fp);	
	return 0;
}
