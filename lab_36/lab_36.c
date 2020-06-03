#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>

#define MAX_PAGES_COUNT 100
#define SCREEN_WIDTH 180
#define SCREEN_HEIGHT 200
#define BUFFER_SIZE SCREEN_HEIGHT * 20
#define POLL_TIMEOUT_USEC 5000
void openFd(int *fd)
{
	if ((*fd = open("/dev/tty", O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	}
}
int setTermios()
{
	int fd = 0; //stdin

	struct termios terminal;

	tcgetattr(fd, &terminal);

	terminal.c_lflag &= ~ICANON;
	terminal.c_cc[VMIN] = 1;
	tcsetattr(fd, TCSANOW, &terminal);

	return fd;
}
void resetTermios(int fd)
{
	struct termios tty;
	tcgetattr(fd, &tty);
	tty.c_lflag |= (ICANON | ECHO);
	tty.c_cc[VMIN] = 4;
	tcsetattr(fd, TCSAFLUSH, &tty);
}

void readHttp(int sockFd, int fd)
{
	int pagesCount = 0;
	int curPageNumber = 0;
	int connectionClosedFlag = 0;
	char pages[MAX_PAGES_COUNT][BUFFER_SIZE];
	int pageSize[MAX_PAGES_COUNT];

	struct pollfd fds[2];
	fds[0].fd = sockFd;
	fds[0].events = POLLIN | POLLHUP;
	fds[0].revents = 0;

	fds[1].fd = fd;
	fds[1].events = POLLIN | POLLHUP;
	fds[1].revents = 0;
	while (1)
	{
		//printf("polling\n");
		int ret = poll(fds, 2, POLL_TIMEOUT_USEC);
		if (ret == -1)
		{
			perror("poll");
			close(sockFd);
			resetTermios(fd);
			exit(2);
		}
		else if (ret == 0)
		{
			close(sockFd);
			connectionClosedFlag = 1;
			//printf("cur: %d, count: %d\n", curPageNumber, pagesCount);
			if (curPageNumber == pagesCount)
			{
				return;
			}
		}
		else
		{
			if ((fds[0].revents & POLLIN) && !connectionClosedFlag)
			{
				if (pagesCount == MAX_PAGES_COUNT - 1)
				{
					printf("No more space in queue :(\n");
					close(sockFd);
					return;
				}
				//we have a page to read
				int len = recv(sockFd, pages[pagesCount], BUFFER_SIZE, 0);
				pageSize[pagesCount] = len;
				pagesCount++;

				if (curPageNumber == 0)
				{
					//show first page
					write(1, pages[curPageNumber], pageSize[curPageNumber]);
					curPageNumber++;
				}
			}
			if (fds[0].revents & POLLHUP)
			{
				//connection is closed on other side
				close(sockFd);
				connectionClosedFlag = 1;
				if (curPageNumber == pagesCount) //if we have no pages to show
					return;
			}

			if (fds[1].revents & POLLIN)
			{
				//printf("cur: %d, count: %d\n", curPageNumber, pagesCount);
				//printf("closed: %d\n", connectionClosedFlag);
				if (curPageNumber == pagesCount) //if we have no pages to show
				{
					if (connectionClosedFlag)
					{
						return;
					}
				}
				else
				{
					char symbol;
					read(fd, &symbol, 1);
					//printf("got %c\n", symbol);
					write(1, pages[curPageNumber], pageSize[curPageNumber]);
					curPageNumber++;
				}
			}
			if (fds[1].revents & POLLHUP)
			{
				//someone want us to exit
				close(sockFd);
				return;
			}

			fds[0].revents = 0;
			fds[1].revents = 0;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Give me adress\n");
		return -1;
	}
	/* Connect to host */
	//get addres by host name
	struct addrinfo hints;
	struct addrinfo *res;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(argv[1], "80", &hints, &res) != 0) {
		perror("getaddrinfo");
		return -1;
	}
	//create socket
	int sockFd;
	if ((sockFd = socket(res->ai_family, res->ai_socktype, 0)) == -1) {
		perror("socket");
		return -1;
	}
	//connect
	if (connect(sockFd, res->ai_addr, res->ai_addrlen) == -1) {
		perror("connect");
		return -1;
	}
	/* Send request */
	//create request
	const char *headerBegin = "GET / HTTP/1.1\r\nHost: ";
	const char *headerEnd = "\r\n\r\n";
	char *header = (char*)malloc((strlen(headerBegin) + strlen(headerEnd) + strlen(argv[1])) * sizeof(char));
	sprintf(header, "%s%s%s", headerBegin, argv[1], headerEnd);
	//send request
	if (send(sockFd, header, strlen(header), 0) == -1) {
		perror("Send error");
		return -1;
	}
	free(header);
	/* Read http */
	//set terminal
	int fd = setTermios();
	//read messages
	readHttp(sockFd, fd);
	//close everithyng
	close(sockFd);
	printf("Connection was closed\n");
	resetTermios(fd);
	return 0;
}
