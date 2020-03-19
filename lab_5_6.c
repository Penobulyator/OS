#include <stdio.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
typedef struct TableHandler
{
	FILE* file;
	int* table;
	int numberOfLines;
}TableHandler;

TableHandler t;

void make_table(FILE* file)
{
	t.file = file;
	t.table= (int*)malloc(100 * sizeof(int));
	t.table[0] = 0;
	t.numberOfLines = 0;
	for (int i = 0; ; i++)
	{
		char c = getc(file);
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
	fseek(t.file, 0, SEEK_SET);
	for (int i = 0; i < t.numberOfLines; i++)
	{
		char s[100];
		if (fscanf(t.file, "%s", s) > 0)
			printf("%s\n", s);
		else
		{
			printf("READ ERROR\n");
			exit(0);
		}
	}
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
				if (fseek(t.file, t.table[num - 1], SEEK_SET))
				{
					printf("Some fseek error\n");
					return;
				}
				fgets(s, 100, t.file);
				printf("%s", s);
			}
		}
		free(s);
	}
}
int main()
{
	FILE* fin = fopen("in.txt", "r");
	make_table(fin);
	print(fin);
	fclose(fin);
	return 0;
}