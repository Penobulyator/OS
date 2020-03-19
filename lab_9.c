
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char *argv[]) {
	pid_t proc;

	if ((proc = fork()) > 0) {
		//we are parent
		printf("Cat is working...\n");
		wait(0);
		printf("Cat has finished working\n");
		return 0;
	}
	else if (proc == 0) {
		//we are child
		system("cat irtegov.txt");
	}
	else if (proc < 0)
	{
		printf("fork has failed\n");
	}
	return 0;
}