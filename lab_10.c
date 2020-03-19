#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char *argv[]) {
	pid_t proc;
	if ((proc = fork()) > 0) {
		//we are parent
		printf("Statring child process...\n");
		int status;
		waitpid(proc, &status, NULL); 
		if (WIFEXITED(status) == 0)
			printf("Some problems with child...\n");
		else
			printf("Child has finished working and returned %d\n", WEXITSTATUS(status));
		return 0;
	}
	else if (proc == 0) {
		//we are child
		if (argc == 3)
			execl(argv[1], argv[1], argv[2], NULL); // <= replasing this process with programm with name "arg[1]"
		else
		{
			printf("Give me 2 arguments\n");
			return 255;
		}
	}
	else if (proc < 0)
	{
		printf("fork has failed\n");
	}
	return 0;
}