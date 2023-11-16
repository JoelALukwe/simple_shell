#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 100

extern char **environ;

void print_environment(char **env)
{
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
void handle_ctrl_d(char *command)
{
	printf("\n");
	free(command);
}
void execute_command(char *args[], char *path)
{
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
	} else if (child_pid == 0)
	{
		execvp(args[0], args);
		fprintf(stderr, "./shell: %s: No such file or directory\n", args[0]);
		_exit(127);
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
void search_and_execute(char *command)
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

	char *path = getenv("PATH");
	char *path_copy = strdup(path);

	token = strtok(path_copy, ":");

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

	if (command_found)
	{
		execute_command(args, path);
	} else
	{
		fprintf(stderr, "./shell: %s: command not found\n", args[0]);
	}
}
int main(void)
{
	char *command = NULL;
	size_t len = 0;

	while (1)
	{
		printf("$ ");
		ssize_t read_len = getline(&command, &len, stdin);

		if (read_len == -1)
		{
			handle_ctrl_d(command);
			break;
		}
		if (read_len > 1)
		{
			command[read_len - 1] = '\0';
			if (strcmp(command, "exit") == 0)
			{
				free(command);
				break;
			} else if (strcmp(command, "env") == 0)
			{
				print_environment(environ);
			} else
			{
				search_and_execute(command);
			}
		}
	}

	free(command);
	return (0);
}
