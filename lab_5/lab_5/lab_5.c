#include <stdio.h>
#include <malloc.h>
int* make_table(FILE* fin)
{
	int* out = (int*)malloc(100 * sizeof(int));
	out[0] = 0;
	int count = 1;
	for (int i = 0; ; i++)
	{
		char c = getc(fin);
		if (c == EOF)
			return out;
		if (c == '\n')
		{
			out[count] = i + 1;
			count++;
		}
	}
}
void print(FILE* fin)
{
	int *table = make_table(fin);
	int num = 0;
	char s[100];
	while (1)
	{
		printf("Enter line number:\n");
		scanf("%d", &num);
		if (num == 0)
			return;
		fseek(fin, table[num - 1], SEEK_SET);
		fgets(s, 100, fin);
		printf("%s", s);
	}
}
int main()
{
	FILE* fin = fopen("in.txt", "r");
	print(fin);
	return 0;
}