#include "list.h"
#define _CRT_SECURE_NO_WARNINGS
int main()
{
	list *head = NULL;
	head = read(head);

	FILE* fout = fopen("file.txt", "w");
	print_to_file(head, fout);
	fclose(fout);

	FILE* fin = fopen("file.txt", "r");
	head = read_from_file(head, fin);
	fclose(fin);
	printf("I'm here\n");
	print(head);
	delete_list(head);
	return 0;
}