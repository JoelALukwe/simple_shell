#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

/**
 *prompt - display the string "($)" followed by a space on the standard output.
 *
 *write: system call to output
 */
void prompt(void)
{
	 write(STDOUT_FILENO, "($) ", 4);
}

/**
 *executeCommand - designed to execute a command specified by the input string
 *
 *@line: The function takes a character array (char *) as input
 *_exit: This line is executed only if execve fails.
 */
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

/**
 * main - Entry point
 *
 * Description: 'the program's description'
 * @argc: parameter number 1
 * @argv: parameter number 2
 *
 * Return: Always 0 (Success)
 */

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

