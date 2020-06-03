#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include <sys/types.h>
#include <unistd.h>

#include <ulimit.h>

#include <sys/resource.h>

extern char ** environ;
int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Nothing to do\n");
		return 0;
	}
	
	const char optString[] = "ispuU:cC:d:vV:";

	char result = 0;

	struct rlimit rlim;

	char buffer[1024];

	long value;
	while ((result = getopt(argc, argv, optString)) != -1) {
		switch (result) {
		case 'i':
			//Печатает реальные и эффективные идентификаторы 
			//пользователя и группы.
			printf("Real UID: %d\n", getuid());
			printf("Effective UID: %d\n", geteuid());
			printf("Real GID: %d\n", getgid());
			printf("Effective GID: %d\n", getegid());
			break;
		case 's':
			//Процесс становится лидером группы
			setpgid(0, 0);
			break;
		case 'p':
			//Печатает идентификаторы процесса, процесса - родителя и
			//группы процессов.
			printf("PID:= %d\n", getpid());
			printf("Parent's PID: %d\n", getppid());
			printf("PGID: %d\n", getpgid(0));
			break;
		case 'u':
			//Печатает значение ulimit
			printf("Ulimit size\t %ld\n", ulimit(UL_GETFSIZE));
			break;
		case 'U':;
			//Изменяет значение ulimit
			value = atol(optarg);
			if (value == 0)
				printf("Wrong -U value\n");
			else
				ulimit(UL_SETFSIZE, value);
			break;
		case 'c':
			//Печатает размер в байтах core-файла, который может быть
			//создан
			getrlimit(RLIMIT_CORE, &rlim);
			printf("rlim_cur:  %d\n", rlim.rlim_cur);
			printf("rlim_max:  %d\n", rlim.rlim_max);
			break;

		case 'C':
			//Изменяет размер core-файла
			value = atol(optarg);
			if (value == 0) 
				printf("Wrong value\n");
			else {
				rlim.rlim_cur = value;
				setrlimit(RLIMIT_CORE, &rlim);
			}
			break;
		case 'd':
			//Печатает текущую рабочую директорию
			getcwd(buffer, 1024);
			printf("Current working directory: %s\n", buffer);
			break;
		case 'v':
			//Распечатывает переменные среды и их значения
			for (int i = 0; environ[i] != NULL; i++)
				printf("\n%s", environ[i]);
			break;
		case 'V':;
			//Вносит новую переменную в среду или изменяет
			//значение существующей переменной.
			char* env_name = strtok(optarg, "=");
			char* env_value = optarg + strlen(env_name) + 1;

			setenv(env_name, env_value, 1);

			printf("%s value is %s\n", env_name, getenv(env_name));
			break;
		}
	}
}