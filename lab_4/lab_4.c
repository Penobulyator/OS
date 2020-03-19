#include "list.h"
#define _CRT_SECURE_NO_WARNINGS
int main()
{
	list *head = NULL;
	head = read(head);

	FILE* fout = fopen("file.txt", "w");
	if (fout == NULL)
	{
		printf("error\n");
		return 1;
	}
	print_to_file(head, fout);
	fclose(fout);

	FILE* fin = fopen("file.txt", "r");
	if (fin == NULL)
	{
		printf("error\n");
		return 1;
	}
	head = read_from_file(head, fin);
	fclose(fin);
	print(head);
	delete_list(head);
	return 0;
}