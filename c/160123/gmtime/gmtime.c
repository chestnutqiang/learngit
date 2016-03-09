#include<time.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
   time_t timep;
	struct tm *pTm;
	time(&timep);
	pTm = gmtime(&timep);
	printf("%d-%d-%d,%d:%d:%d\n", pTm->tm_year+1900,pTm->tm_mon+1,pTm->tm_mday,pTm->tm_hour,pTm->tm_min,pTm->tm_sec);
	return 0;
}
