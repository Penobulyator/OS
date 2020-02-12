#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
void print_id()
{
	printf("Read user id: %d\n", getuid());
	printf("Effective user id: %d\n", geteuid());
}
void try_to_open()
{
	FILE* f = fopen("lab_3.file", "rb");
	if (f == NULL) 
	{
		perror("File not open\n");
	}
	else 
	{
		printf("File opened successfully\n");
	}
	fclose(f);
}
int main() 
{
	printf("Before changing uid:\n");
	print_id();
	try_to_open();

	seteuid(getuid());

	printf("After changing uid:\n");
	print_id();
	try_to_open();
}
