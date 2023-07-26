#include "shell.h"

/**
 * find_built_in_command - finds a built-in command
 * @info: the parameter & return info struct
 *
 * Return: -1 if built-in command not found,
 * 	0 if built-in command executed successfully,
 * 	1 if built-in command found but not successful,
 * 	2 if built-in command signals exit()
 */
int find_built_in_command(info_t *info)
{
	int i, built_in_command_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_command_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_command_ret);
}
/**
 * fork_cmd - forks a child process to run the command
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	info_t info = { INFO_INIT };
	int builtin_ret = 0;

	if (argc > 1)
		return (1);

	hsh(&info, argv);
	return (builtin_ret);
}
