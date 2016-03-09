#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<strings.h>
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
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("args is error !\n");
		return -1;
	}
	DIR *dir;
	dir = opendir(argv[1] );
	if(dir == NULL)
	{
		perror("opendir");
		return -1;
	}
	struct dirent * pdir;
	struct stat  st1;
	int ret;
	char buf[50];
	while( pdir= readdir(dir))
	{
		if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
		{
			continue;
		}
		bzero(buf, sizeof(buf));
		sprintf(buf,"%s%s",argv[1], pdir->d_name); 
		ret = stat( buf, &st1);	
		if(ret == -1)
		{
			perror("stat");
			return -1;
		}
		char buf1[50];
		aut(st1.st_mode, buf1);	
	//printf("%s\n", buf);
		printf("%-11s%3ld%15s%15s%10ld%15s%10s\n",buf1,st1.st_nlink,getpwuid(st1.st_uid)->pw_name,
 getgrgid(st1.st_gid)->gr_name,st1.st_size,substr(ctime(&st1.st_mtime)), pdir->d_name);
	}
	return 0;
}
