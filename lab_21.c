#include <signal.h>
#include <stdio.h>
int soundCount = 0;
int periodCount = 0;
void makeSound()
{
	puts("\7");
	soundCount++;
}
void quit()
{
	printf("There was %d sounds and %d periods\n", soundCount, periodCount);
	exit(0);
}
void incTimeCount()
{
	periodCount++;
	alarm(2);
}
int main() 
{
	alarm(2);
	while(1)
	{
		signal(SIGALRM, incTimeCount);
		signal(SIGINT, makeSound);
		signal(SIGQUIT, quit);
	}
}