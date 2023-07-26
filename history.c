#include "shell.h"

/**
 * get_history_file - gets the path to the history file
 * @info: pointer to the parameter struct
 *
 * Return: a string containing the path to the history file
 */
char *get_history_file(info_t *info)
{
	char *dir_path, *file_path;

	dir_path = _getenv(info, "HOME=");
	if (!dir_path)
		return (NULL);
	file_path = malloc(sizeof(char) * (_strlen(dir_path) + _strlen(HIST_FILE) + 2));
	if (!file_path)
		return (NULL);
	file_path[0] = '\0';
	_strcpy(file_path, dir_path);
	_strcat(file_path, "/");
	_strcat(file_path, HIST_FILE);
	return (file_path);
}

/**
 * write_history - writes the command history to a file
 * @info: pointer to the parameter struct
 *
 * Return: 1 on success, -1 on failure
 */
int write_history(info_t *info)
{
	ssize_t fd;
	char *file_path = get_history_file(info);
	list_t *node = NULL;

	if (!file_path)
		return (-1);

	fd = open(file_path, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_path);
	if (fd == -1)
		return (-1);
	for (node = info->command_history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads the command history from a file
 * @info: pointer to the parameter struct
 *
 * Return: the number of commands read on success, 0 on failure
 */
int read_history(info_t *info)
{
	int i, last = 0, command_count = 0;
	ssize_t fd, rdlen, file_size = 0;
	struct stat st;
	char *buf = NULL, *file_path = get_history_file(info);

	if (!file_path)
		return (0);

	fd = open(file_path, O_RDONLY);
	free(file_path);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		file_size = st.st_size;
	if (file_size < 2)
		return (0);
	buf = malloc(sizeof(char) * (file_size + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, file_size);
	buf[file_size] = '\0';
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < file_size; i++)
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			add_command_to_history(info, buf + last, command_count++);
			last = i + 1;
		}
	if (last != i)
		add_command_to_history(info, buf + last, command_count++);
	free(buf);
	info->command_count = command_count;
	while (info->command_count-- >= MAX_COMMAND_HISTORY)
		delete_node_at_index(&(info->command_history), 0);
	renumber_history(info);
	return (info->command_count);
}

/**
 * add_command_to_history - adds a command to the command history list
 * @info: pointer to the parameter struct
 * @command: the command to add to the history list
 * @command_count: the current number of commands in the history list
 *
 * Return: Always 0
 */
int add_command_to_history(info_t *info, char *command, int command_count)
{
	list_t *node = NULL;

	if (info->command_history)
		node = info->command_history;
	add_node_end(&node, command, command_count);

	if (!info->command_history)
		info->command_history = node;
	return (0);
}

/**
 * renumber_history - renumbers the command history list after changes
 * @info: pointer to the parameter struct
 *
 * Return: the new number of commands in the command history list
 */
int renumber_history(info_t *info)
{
	list_t *node = info->command_history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->command_count = i);
}
