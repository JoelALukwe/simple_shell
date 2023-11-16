#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

void prompt(void)
{
	 write(STDOUT_FILENO, "($) ", 4);
}

void executeCommand(char *line)
{
	char *token;
	char *args[BUFSIZE];
	int arg_count = 0;

	token = strtok(line, " \n");
	while (token != NULL)
	{
		args[arg_count++] = token;
		token = strtok(NULL, " \n");
	}
	args[arg_count] = NULL;

	execve(args[0], args, NULL);
	perror("execve");
	_exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t len = 0;

	if (isatty(STDIN_FILENO))
	{
		prompt();
	}
	while (getline(&line, &len, stdin) != -1)
	{
		if (strcmp(line, "exit\n") == 0)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(0);
		}

		pid_t children_pid = fork();

		if (children_pid == -1)
		{
			perror("fork");
		}
		else if (children_pid == 0)
		{
			executeCommand(line);
		}
		else
		{
			wait(NULL);
		}

		if (isatty(STDIN_FILENO))
		{
			prompt();
		}
	}

	free(line);
	return (0);
}

