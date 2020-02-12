#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];
int main()
{
	setenv("TZ", "America/Dawson", 1);

	struct tm *sp;
	char str[100];
	time_t now = time(NULL);
	ctime_r(&now, str, 100);
	printf("%s	", str);
	sp = localtime(&now);
	printf("%d/%d/%02d %d:%02d %s\n",
	sp->tm_mon + 1, sp->tm_mday,
	sp->tm_year + 1900, sp->tm_hour,
	sp->tm_min, tzname[sp->tm_isdst]);
	exit(0);
}