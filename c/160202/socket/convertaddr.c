#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
	char ip[] =  "192.168.0.101";

	struct in_addr myaddr;
	int ret = inet_aton(ip, &myaddr);
	printf("%x\n", myaddr.s_addr);
	
	printf("%x\n", inet_addr(ip));
	
	printf("%s\n", inet_ntoa(myaddr));
	
	printf("%d\n", inet_pton(AF_INET, ip, &myaddr));
	printf("%x\n", myaddr.s_addr);
	memset(ip, 0, sizeof(ip));
	inet_ntop(AF_INET, &myaddr, ip, 16);
	puts(ip);
	return 0;

}
