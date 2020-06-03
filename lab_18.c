#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include <unistd.h>

#include <pwd.h>
#include <grp.h>

#include <sys/types.h>

#include <sys/stat.h>

#define bool int
bool isDir(struct stat fileStat)
{
	return S_ISDIR(fileStat.st_mode);
}

bool isRegularFile(struct stat fileStat)
{
	return S_ISREG(fileStat.st_mode);
}

void printStatus(struct stat fileStat)
{
	//печатает биты состояния файла в воспринимаемой человеком форме:
	//d если файл является каталогом
	//- если файл является обычным файлом
	//? во всех остальных случаях
	if (isDir(fileStat))
		printf("d");
	else if (isRegularFile(fileStat))
		printf("-");
	else
		printf("?");
}

void printOwnerRights(struct stat fileStat)
{
	if (fileStat.st_mode & S_IRUSR)
		printf("r");
	else
		printf("-");


	if (fileStat.st_mode & S_IWUSR)
		printf("w");
	else
		printf("-");


	if (fileStat.st_mode & S_IXUSR)
		printf("x");
	else
		printf("-");

	printf("-");
}

void printGroupRights(struct stat fileStat)
{
	if (fileStat.st_mode & S_IRGRP)
		printf("r");
	else
		printf("-");


	if (fileStat.st_mode & S_IWGRP)
		printf("w");
	else
		printf("-");


	if (fileStat.st_mode & S_IXGRP)
		printf("x");
	else
		printf("-");

	printf("-");
}

void printOthersRights(struct stat fileStat)
{
	if (fileStat.st_mode & S_IROTH)
		printf("r");
	else
		printf("-");


	if (fileStat.st_mode & S_IWOTH)
		printf("w");
	else
		printf("-");


	if (fileStat.st_mode & S_IXOTH)
		printf("x");
	else
		printf("-");

	printf("-");
}

void printAccessRights(struct stat fileStat)
{
	//печатает три группы символов, соответствующие правам доступа для хозяина, группы и всех остальных :
	//r если файл доступен для чтения, иначе -
	//w если файл доступен для записи, иначе -
	//x если файл доступен для исполнения, иначе -
	printOwnerRights(fileStat);
	printGroupRights(fileStat);
	printOthersRights(fileStat);
}

void printOwner(uid_t uid)
{
	struct passwd *p = getpwuid(uid);
	printf("%s ", p->pw_name);
}

void printGroup(gid_t  gid)
{
	struct group *g = getgrgid(gid);
	printf("%s ", g->gr_name);
}

void printLinks(struct stat fileStat)
{
	//печатает количество связей файла :
	//имена собственника и группы файла
	//Если файл является обычным файлом, его размер.
	//Дату модификации файла(используйте ctime).
	//Имя файла(если было задано имя с путем, нужно распечатать только имя).
	printOwner(fileStat.st_uid);
	printGroup(fileStat.st_gid);

	if (isRegularFile(fileStat))
		printf("%d ", fileStat.st_size);

	printf("%s ", ctime(&fileStat.st_mtime));
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Nothing to do\n");
		return 0;
	}

	const char optString[] = "sal";

	char cur_dir[255];
	if (getcwd(cur_dir, sizeof(cur_dir)) == 0)
		perror("getcwd");

	char filename[255];
	sprintf(filename, "%s/%s", cur_dir, "lab_8.file");


	struct stat fileStat;
	if (stat(filename, &fileStat) < 0)
	{
		printf("stat error\n");
		return 0;
	}

	char result = 0;
	while ((result = getopt(argc, argv, optString)) != -1) {
		switch (result) {
		case 's':
			printStatus(fileStat);
			break;

		case 'a':
			printAccessRights(fileStat);
			break;
		case 'l':
			printLinks(fileStat);
			break;
		}
		printf("\t");
	}
	printf("\n");
	return 0;
}