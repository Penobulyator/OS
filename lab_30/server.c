#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <string.h>

#include <protocol.h>

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_un serv_addr;
	//create socket
	if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0) ) < 0)
	{
		perror("socket");
		exit(1);
	}

    serv_addr.sun_family = AF_UNIX;
	unlink(ADDRESS);
	strcpy(serv_addr.sun_path, ADDRESS);
	//bind socket to some address
	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("bind");
		exit(2);
	}
	
    listen(listenfd, 1);

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        
	char buf[MSGSIZE];
    int len = read(connfd, buf, MSGSIZE);
	for (int i = 0; i < len; i++)
		if (islower(buf[i]))
			buf[i] = toupper(buf[i]);
	printf("%s", buf);
    close(connfd);
	return 0;
} 