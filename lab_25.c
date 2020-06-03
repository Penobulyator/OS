#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MSGSIZE 20
int main(int argc, char *argv[]) {
	pid_t proc;
	int fd[2]; 
	char *buf;
	if (pipe(fd) == -1) {
		perror("pipe");
	}
	if ((proc = fork()) > 0) {
		//we are parent
		close(fd[0]);
		write(fd[1], "HeLLo cHiLD\n", MSGSIZE);

		close(fd[1]);
		return 0;
	}
	else if (proc == 0) {
		//we are child
		close(fd[1]);

		int len = read(fd[0], buf, MSGSIZE);
		for (int i = 0; i < len; i++)
			if (islower(buf[i]))
				buf[i] = toupper(buf[i]);
		printf("%s", buf);

		close(fd[0]);
	}
	else if (proc < 0)
	{
		printf("fork has failed\n");
	}
	return 0;
}