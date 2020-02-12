#include <malloc.h>
#include <string.h>
#include <stdio.h>
typedef struct list {
	char string[100];
	struct list* next;
} list;
list* push_back(list* head, char* str) 
{
	list *t = (list*)malloc(sizeof(list));
	strcpy(t->string, str);
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