#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	char ch;
	struct termios terminal;
	struct termios save_set;

	tcgetattr(STDIN_FILENO, &terminal);
	save_set = terminal;

	terminal.c_lflag &= ~ICANON;
	terminal.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);

	write(STDOUT_FILENO, "Enter some symbol: ", 19);
	read(STDIN_FILENO, &ch, 1);
	printf("\nYou entered: %c\n", ch);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &save_set);
	return 0;
}