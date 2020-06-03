#include <stdio.h>
#include <ctype.h>
#include <libgen.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *fpin, *fpout[2];
    char line[2];
    if ((fpin = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
        exit(1);
    }

	if (p2open("wc -l", fpout) == -1)
	{
		printf("p2open failed\n");
		exit(1);
	}
 
    char currentSymbol = 0;
	
    while (feof(fpin) == 0) {
        int length = 0;
        while ((currentSymbol = fgetc(fpin)) != '\n' && !feof(fpin)) {
			length++;
        }
        if (length == 0) {
           fputs(line, fpout[0]);
		}   
    }
    pclose(fpout[0]);

    char wc_output[100];
    fgets(wc_output, 100, fpout[1]);

    int number_offset = 0;
    while (wc_output[number_offset] == ' ')
		number_offset++;

    printf("Empty lines amount : %d\n", atoi(wc_output + number_offset));
    fclose(fpin);
    pclose(fpout[1]);
}
