#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>

void printdir(char *dir, int depth)
{
	DIR *pd;
	pd = opendir(dir);
	if(pd == NULL)
	{
		printf("open file is fail\n");
		exit(1);
	} 
	chdir(dir);
	struct dirent *entry;
	struct stat statbuf;
	while((entry = readdir(pd)) != NULL )
	{
		stat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
				continue;
			printf("%*s%s/\n",depth,"",entry->d_name);
			printdir(entry->d_name, depth+4);
		}else{
			printf("%*s%s\n",depth,"",entry->d_name);
		}
	}
	chdir("..");
	closedir(pd);
}


int main(int argc, char *argv[])
{
	printdir(argv[1],0);
	return 0;		
}
