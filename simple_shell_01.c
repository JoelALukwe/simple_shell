#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
void handle_parent_process(char *command)
{
	int status;

	wait(&status);
	if (WIFSIGNALED(status))
	{
		printf("\n");
	}
}
int main(void)
{
	char *command = NULL;
	size_t len = 0;

	while (1)
	{
		printf("#cisfun$ ");
		ssize_t read_len = getline(&command, &len, stdin);

		if (read_len == -1)
		{
			handle_ctrl_d(command);
		}

		if (read_len > 1)
		{
			command[read_len - 1] = '\0';
		}

		pid_t child_pid = fork();

		if (child_pid == -1)
		{
			perror("fork");
		}
		else if (child_pid == 0)
		{
			handle_child_process(command);
		} else
			handle_parent_process(command);
	}
	free(command);
	return (0);
}
