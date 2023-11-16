#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100

void readInput(char **command, size_t *len)
{
	printf("#cisfun$ ");
	ssize_t read_len = getline(command, len, stdin);

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

void executeCommand(char *command)
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

	pid_t child_pid = fork();

	if (child_pid == -1)
	{
	perror("fork");
	}
	else if (child_pid == 0)
	{
		execvp(args[0], args);
		fprintf(stderr, "./shell: %s: No such file or directory\n", args[0]);
		exit(127);
	} else
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
			executeCommand(command);
		}
	}

	free(command);
	return (0);
}

