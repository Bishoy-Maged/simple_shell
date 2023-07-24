#include "shell.h"

/**
 * shell_history - Displays the history list, one command per line, preceded
 *                 with line numbers, starting at 0.
 * @info: Pointer to the info_t struct. Used to maintain
 *        a constant function prototype.
 *
 * Return: Always 0.
 */
int shell_history(info_t *info)
{
	print_list_strings(info->history);
	return (0);
}

/**
 * unset_alias - Unsets alias for a given string.
 * @info: Pointer to the info_t struct.
 * @str: The string alias to unset.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_from_list_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - Sets alias to a given string.
 * @info: Pointer to the info_t struct.
 * @str: The string alias to set.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_to_list_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints an alias string.
 * @node: The alias node.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * shell_alias - Mimics the alias builtin (man alias).
 * @info: Pointer to the info_t struct. Used to maintain
 *          a constant function prototype.
 *
 * Return: Always 0.
 */
int shell_alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}

	for (i = 1; info->argv[i]; i++)
	{
		p = _strchr(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(find_node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
