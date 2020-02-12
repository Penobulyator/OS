#include "list.h"
int main()
{
	list *head = NULL;
	char cur_str[100];
	gets(cur_str);
	while (strcmp(cur_str, "."))
	{
		head = push_back(head, cur_str);
		gets(cur_str);
	}
	print(head);
	delete_list(head);
	return 0;
}