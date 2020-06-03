#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <aio.h>
#include <protocol.h>
#include <aio.h>
char messages[CLIENTS_COUNT][MSGSIZE];
int closedFdCount = 0;
void sigiohandler(int signo, siginfo_t *info, void *context) {
	struct aiocb * req;
	if (signo != SIGIO || info->si_signo != SIGIO) 
		return;
	req = (struct aiocb *)info->si_value.sival_ptr;
	if (aio_error(req) == 0) {
		/* it's complete!!! */
		size_t size = aio_return(req);
		if (size == 0) {
			/* EOF */
			close(req->aio_fildes);
			closedFdCount++;
			if (closedFdCount == CLIENTS_COUNT)
			{
				printf("All clients are disconected\n");
				exit(0);
			}
		}
		else
		{
			char* buf = (char*)req->aio_buf;
			for (int i = 0; i < size; i++)
				if (islower(buf[i]))
					buf[i] = toupper(buf[i]);
			write(1, buf, size);
			/* schedule next read */
			int ret = aio_read(req);
			if (ret == -1)
			{
				printf("Some problem\n");
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
	
	//set signal handler
	struct sigaction sigiohandleraction;
	memset(&sigiohandleraction, 0, sizeof(struct sigaction));
	sigiohandleraction.sa_sigaction = sigiohandler;
	sigiohandleraction.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &sigiohandleraction, NULL);

	//accept clients
	struct aiocb fds[CLIENTS_COUNT];
    listen(listenfd, 10); 
	for (int i = 0; i < CLIENTS_COUNT; i++)
	{
		int fd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		memset(&fds[i], 0, sizeof(struct aiocb));
		fds[i].aio_fildes = fd;
		fds[i].aio_buf = messages[i];
		fds[i].aio_nbytes = MSGSIZE;

		struct sigevent sigev;
		memset(&sigev, 0, sizeof(struct sigevent));
		sigev.sigev_notify = SIGEV_SIGNAL;
		sigev.sigev_signo = SIGIO;
		sigev.sigev_value.sival_ptr = &fds[i];

		fds[i].aio_sigevent = sigev;
	}
	for (int i = 0; i < CLIENTS_COUNT; i++)
		aio_read(&fds[i]);

	for (;;);
	return 0;
} 
