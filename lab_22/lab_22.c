#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIZE 11
#define TIMEOUT_USEC 5000 //5 seconds
int main(int argc, char **argv)
{
	const int fdCount = argc - 1;
	int closedFdCount = 0;
	struct pollfd fds[argc - 1];
	for (int i = 0; i < fdCount; i++)
	{
		int fd = open(argv[i + 1], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			exit(1);
		}
		fds[i].fd = fd;
		fds[i].events = POLLIN;
	}
	for (int i=0;;i = (i+1)%fdCount)
	{
		int ret = poll(&fds[i], 1, TIMEOUT_USEC);
		if (ret == -1)
		{
			perror("poll");
			exit(2);
		}
		else if (ret == 0)
		{
			printf("Timeout\n");
			exit(0);
		}
		else
		{
			if (fds[i].revents & POLLIN)
			{
				char buf[SIZE];
				int len = read(fds[i].fd, buf, SIZE);
				if (len == 0)
				{
					/* EOF */
					closedFdCount++;
					close(fds[i].fd);
					if (closedFdCount == fdCount)
					{
						printf("I'm done\n");
						exit(0);
					}
				}
				else
				{
					write(1, buf, len);
				}
			}
			fds[i].revents = 0;
		}
	}
}
