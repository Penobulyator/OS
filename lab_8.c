#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
int main()
{
	struct flock lock;
	FILE	* fd = fopen("lab_8.file", "r+");
	if (fd == NULL)
	{
		printf("Can't open file\n");
		exit(1);
	}
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	if (fcntl(fileno(fd), F_SETLK, &lock) == -1)
	{
		if ((errno == EACCES) || (errno == EAGAIN)) 
		{
			printf("Can't lock file\n");
		}
		else
		{
			printf("Unknown error\n");
		}
		perror("lab_8.file");
		exit(2);
	}
	system("nano lab_8.file");
	lock.l_type = F_UNLCK; /* unlock file */
	fcntl(fileno(fd), F_SETLK, &lock);
	return 0;
}