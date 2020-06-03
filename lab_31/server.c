
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include <protocol.h>

void recvMessages(struct pollfd *fds, int listenfd)
{
	int closedFdCount = 0;
	while (1)
	{
		//printf("polling\n");
		int ret = poll(fds, CLIENTS_COUNT, 3000);
		if (ret == -1)
		{
			for (int i = 0; i < CLIENTS_COUNT; i++)
			{
				close(fds[i].fd);
			}
			close(listenfd);

			perror("select");
			exit(1);
		}
		else if (ret == 0)
		{
			printf("Timeout\n");
			for (int i = 0; i < CLIENTS_COUNT; i++)
			{
				close(fds[i].fd);
			}
			close(listenfd);
			exit(0);
		}
		else if (ret > 0)
		{
			//we've got some event
			for (int i = 0; i < CLIENTS_COUNT; i++)
			{
				if (fds[i].revents & POLLIN)
				{
					char buf[MSGSIZE];
					int len = recv(fds[i].fd, buf, MSGSIZE, 0);
					for (int i = 0; i < len; i++)
						if (islower(buf[i]))
							buf[i] = toupper(buf[i]);
					write(1, buf, len);
				}
				if (fds[i].revents & POLLHUP)
				{
					close(fds[i].fd);
					closedFdCount++;
					//if all clients are closed
					if (closedFdCount == CLIENTS_COUNT)
					{
						close(listenfd);
						printf("All clients are disconected\n");
						exit(0);
					}
				}

				fds[i].revents = 0;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	int listenfd = 0;
    struct sockaddr_un serv_addr;


	//create socket
	if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0) ) < 0)
	{
		perror("socket");
		exit(1);
	}

	//bind socket to some address
    serv_addr.sun_family = AF_UNIX;
	unlink(ADDRESS);
	strcpy(serv_addr.sun_path, ADDRESS);
	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("bind");
		exit(2);
	}
	
	//start listening for clients
	struct pollfd fds[CLIENTS_COUNT];

    listen(listenfd, 10);
	for (int i = 0; i < CLIENTS_COUNT; i++)
	{
		int fd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		fds[i].fd = fd;
		fds[i].events = POLLIN | POLLHUP;
	}

	recvMessages(fds, listenfd);
	return 0;
} 