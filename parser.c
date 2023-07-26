#include "shell.h"

/**
 * find_executable - finds the full path to an executable command
 * @info: the info struct
 * @path_env: the path environment variable
 * @cmd: the command to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_executable(info_t *info, char *path_env, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!path_env)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!path_env[i] || path_env[i] == ':')
		{
			path = dup_chars(path_env, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(info, path))
				return (path);
			if (!path_env[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
