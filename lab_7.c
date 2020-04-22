#include <stdio.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
typedef struct TableHandler
{
	char* file;
	int* table;
	int numberOfLines;
	int length;
}TableHandler;

TableHandler t; 
long filesize(FILE *stream)
{
	long curpos, length;
	/*save current pointer value*/
	curpos = ftell(stream);
	/* go to the end of the file*/
	fseek(stream, 0L, SEEK_END);
	/* get current pointer value */
	length = ftell(stream);
	/* load saved pointer value*/
	fseek(stream, curpos, SEEK_SET);
	return length;
}
void make_table(FILE* file)
{
    t.length = filesize(file); 
    t.file = mmap(0, t.length, PROT_READ, MAP_SHARED, fileno(file), 0);
	if (t.file == MAP_FAILED)
	{
		perror("MAP ERROR");
		exit(0);
	}	
	t.table= (int*)malloc(100 * sizeof(int));
	t.table[0] = 0;
	t.numberOfLines = 0;
	for (int i = 0; i < t.length; i++)
	{
		char c = t.file[i];
		if (c == EOF)
			return;
		if (c == '\n')
		{
			t.table[t.numberOfLines + 1] = i + 1;
			t.numberOfLines++;
		}
	}
}
int printFile()
{
	printf("Time is out\n");
	write(1, t.file, t.length);
	exit(0);
	return 0;
}
char* getWord()
{
	signal(SIGALRM, printFile);
	alarm(5);
	char* out = (char*)malloc(sizeof(char) * 100);
	if (scanf("%s", out) <= 0)
	{
		printf("SCANF ERROR\n");
		exit(0);
	}
	return out;
}
void print()
{
	while (1)
	{
		printf("Enter line number:\n");
		char* s = getWord();
		if (!strcmp(s, "0"))
		{
			return;
		}
		int num = atoi(s);
		if (num == 0)
			printf("Bad format (expected number)\n");
		else
		{
			if (num < 0 || num > t.numberOfLines)
				printf("Bad number\n");
			else
			{
				write(1, t.file + t.table[num - 1], t.table[num] - t.table[num - 1]);
			}
		}
		free(s);
	}
}
int main()
{
	FILE* fin = fopen("lab_7.txt", "r");
	if (fin == NULL)
	{
		perror("cant't open file");
		return 1;
	}
	make_table(fin);
	print(fin);
	fclose(fin);
	return 0;
}