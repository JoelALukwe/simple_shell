#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

void handle_ctrl_d(char *command)
{
	printf("\n");
	free(command);
	exit(EXIT_SUCCESS);
}

void handle_child_process(char *command)
{
	execlp(command, command, (char *)NULL);
	fprintf(stderr, "./shell: %s: No such file or directory\n", command);
	_exit(EXIT_FAILURE);
}

int main(void)
{
	char *command = NULL;

	while (1)
	{
		printf("#cisfun$ ");

		command = (char *)malloc(MAX_INPUT_LENGTH);
		if (command == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		if (fgets(command, MAX_INPUT_LENGTH, stdin) == NULL)
		{
			handle_ctrl_d(command);
		}
		size_t read_len = strlen(command);

		if (read_len > 0 && command[read_len - 1] == '\n')
		{
			command[read_len - 1] = '\0';
		}

		pid_t child_pid = fork();

		if (child_pid == -1)
		{
			perror("fork");
		} else if (child_pid == 0)
		{
			handle_child_process(command);
		}
		free(command);
	}

	return (0);
}
