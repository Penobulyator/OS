#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <protocol.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_un serv_addr;

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, ADDRESS);
	
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

	//send messages
	srand(time(NULL)*	getpid());
	for (int i = 0; i < MSGCOUNT; i++)
	{
		char buf[MSGSIZE];
		sprintf(buf, "MeSSage NUmbEr %d\n", i);
		send(sock, buf, strlen(buf), 0);
		//printf("Sended %d\n", i);
		//sleep random time from 1.7 seconds
		usleep(1000000 * 1.7);
	}
	shutdown(sock, SHUT_RDWR);
    return 0;
}