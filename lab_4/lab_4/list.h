#include <malloc.h>
#include <string.h>
#include <stdio.h>
typedef struct list {
	char string[100];
	struct list* next;
} list;
void my_strcpy(char* buf, char* source)
{
	for (char* c = source; *c != '\0'; c++)
	{
		*(buf + (c - source)) = *c;
	}
}
list* push_back(list* head, char* str) 
{
	list *t = (list*)malloc(sizeof(list));
	my_strcpy(t->string, str);
	t->next = head;
	return t;
}
void print(list* head)
{
	if (head == NULL)
		return;
	else
	{
		print(head->next);
		printf("%s\n", head->string);
	}
}
void print_to_file(list* head, FILE* fout)
{
	if (head == NULL)
		return;
	else
	{
		print_to_file(head->next, fout);
		fprintf(fout,"%s\n", head->string);
	}
}
list* read(list*head)
{
	char cur_str[100];
	gets(cur_str);
	while (strcmp(cur_str, "."))
	{
		head = push_back(head, cur_str);
		gets(cur_str);
	}
	return head;
}
list* read_from_file(list *head, FILE* fin)
{
	char cur_str[100]; 
	while (fgets(cur_str, 100, fin) != NULL)
	{	
		cur_str[strlen(cur_str) - 1] = '\0'; //discard '\n'
		head = push_back(head, cur_str);
	}
	return head;
}
void delete_list(list *head)
{
	while (1)
	{
		if (head == NULL)
			return;
		else
		{
			list* next = head->next;
			free(head);
			head = next;
		}
	}
}