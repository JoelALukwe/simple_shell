#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100

void readInput(char **command, size_t *len)
{
	ssize_t read_len;

	printf(":) ");
	read_len = getline(command, len, stdin);

	if (read_len == -1)
	{
		printf("\n");
		free(*command);
		exit(0);
	}

	if (read_len > 1)
	{
		(*command)[read_len - 1] = '\0';
	}
}

int searchCommand(char *args[])
{
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *token = strtok(path_copy, ":");

	int command_found = 0;

	while (token != NULL)
	{
		char cmd_path[100];

		snprintf(cmd_path, sizeof(cmd_path), "%s/%s", token, args[0]);
		if (access(cmd_path, X_OK) == 0)
		{
			command_found = 1;
			break;
		}
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (command_found);
}

void executeCommand(char *args[])
{
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
	}
	else if (child_pid == 0)
	{
		execvp(args[0], args);
		fprintf(stderr, "./shell: %s: No such file or directory\n", args[0]);
		_exit(127);
	}
	else
	{
		int status;

		wait(&status);
		if (WIFSIGNALED(status))
		{
			printf("\n");
		}
	}
}

int main(void)
{
	char *command = NULL;
	size_t len = 0;

	while (1)
	{
		readInput(&command, &len);

		if (strlen(command) > 0)
		{
			char *args[MAX_ARGS];
			char *token = strtok(command, " ");
			int arg_count = 0;

			while (token != NULL)
			{
				args[arg_count++] = token;
				token = strtok(NULL, " ");
			}

			args[arg_count] = NULL;

			if (searchCommand(args))
			{
				executeCommand(args);
			}
			else
			{
				fprintf(stderr, "./shell: %s: command not found\n", args[0]);
			}
		}
	}

	free(command);
	return (0);
}

