#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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

bool isDir(const char *path)
{
	struct stat path_stat;
	if (stat(path, &path_stat) < 0)
		printf("stat error\n");
	return S_ISDIR(path_stat.st_mode);
}
int pos(char* str, char symbol)
{
	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] == symbol)
			return i;
	return -1;
}
char* substr(char* s, size_t pos, size_t count)
{
	static char buf[BUFSIZ];
	memset(buf, '\0', BUFSIZ);
	return strncpy(buf, s + pos, count);
}
void find(char* path, char* mask)
{
	DIR *dir;
	struct dirent *entry;
	dir = opendir(path);
	if (!dir) {
		perror("diropen");
		exit(1);
	};
	int slash_pos = pos(mask, '/');
	if (slash_pos != -1) //if we need to go to another directory
	{
		char dir_mask[255];
		for (int i = 0; i < slash_pos; i++)
			dir_mask[i] = mask[i];
		while ((entry = readdir(dir)) != NULL) {
			if (check(entry->d_name, dir_mask) == TRUE)
			{
				char new_path[255];
				sprintf(new_path, "%s/%s", path, entry->d_name);
				printf("\n%s:\n", entry->d_name);
				find(new_path, mask + slash_pos + 1);
				printf("\n");
			}
		}

	}
	else
	{
		while ((entry = readdir(dir)) != NULL) {
			if (check(entry->d_name, mask) == TRUE)
			{
					printf("%s\n", entry->d_name);
			}
		}
	}
	closedir(dir);
}
int main(int argc, char** argv) {

	char cur_dir[255];
	if (getcwd(cur_dir, sizeof(cur_dir)) == 0)
		perror("getcwd");
	find(cur_dir, argv[1]);
	return 0;
}