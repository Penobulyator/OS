#include <sys/types.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    int i;
    FILE* fd;
    char num[4];

    srand(time(NULL));
	fd = popen("/bin/sort", "w");

    for (i = 1; i <= 100; i++) {
        sprintf(num, "%02d\n", rand() % 100);
        fputs(num, fd);
    }
	printf("\n");
    pclose(fd);

	//int *stat_loc;
	//wait(stat_loc);

	printf("\n");
}
