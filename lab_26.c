#include <stdio.h>
#define MSGSIZE 20
#define MSGCOUNT 5
int main() {
	char messages[MSGCOUNT][MSGSIZE]= { "meSSege1\n", "meSSege2\n", "meSSege3\n", "meSSege4\n", "meSSege5\n" };
    FILE* fptr = popen("./lab_26_child.out", "w");
	for (int i = 0; i < MSGCOUNT; i++)
		fputs(messages[i], fptr);
    pclose(fptr);
	return 0;
}
	