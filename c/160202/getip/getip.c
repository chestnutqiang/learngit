#include<netdb.h>
#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
int main(int argc, char*argv[])
{
	char *ptr,**pptr;
	struct hostent *hptr;
	ptr = argv[1];
	if((hptr = gethostbyname(ptr))==NULL )
	{
		printf("gethostbyname error for host : %s\n", ptr);
		return 0;
	}

	printf("official hostname:%s\n", hptr->h_name);
	for( pptr=hptr->h_aliases; *pptr != NULL; pptr++)
		printf("aliases : %s \n", *pptr);
	char str[32] = {0};
	for(pptr=hptr->h_addr_list; *pptr!=NULL;pptr++)
	{
		printf("address:%s\n", inet_ntop(hptr->h_addrtype, *pptr,str,sizeof(str)));
	}
	return 0;
}
