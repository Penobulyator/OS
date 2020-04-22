#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
typedef unsigned short bool;
bool check(char *s, char *mask)
{
	char *rs = 0, *rp;
	char *p = (char*)malloc(sizeof(char) * strlen(mask));
	strcpy(p, mask);
	while (1)
		if (*p == '*')
			rs = s, rp = ++p;
		else if (!*s)
			return !*p;
		else if (*s == *p || *p == '?')
			++s, ++p;
		else if (rs)
			s = ++rs, p = rp;
		else
			return FALSE;
}
int main(int argc, char** argv) {
	DIR *dir;
	struct dirent *entry;
	char cur_dir[255];
	bool was_good_file = FALSE;
	if (getcwd(cur_dir, sizeof(cur_dir)) == 0)
		perror("getcwd");
	dir = opendir(cur_dir);
	if (!dir) {
		perror("diropen");
		exit(1);
	};
	while ((entry = readdir(dir)) != NULL) {
		if (check(entry->d_name, argv[1]) == TRUE)
		{
			was_good_file = TRUE;
			printf("%s\n", entry->d_name);
		}
	}
	closedir(dir);
	if (was_good_file == FALSE)
	{
		printf("%s\n", argv[1]);
	}
	return 0;
}