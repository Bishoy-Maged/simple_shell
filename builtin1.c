#include "shell.h"

/**
 * _myhistory - Display the history list, one command per line, with line numbers starting at 0.
 * @info: Structure containing potential arguments.
 *
 * Print the history list containing previously executed commands, each followed by a line number.
 * Return: Always 0.
 */
int _myhistory(info_t *info)
{
    print_list(info->history);
    return (0);
}

/**
 * unset_alias - Unset an alias by deleting it from the alias list.
 * @info: Parameter struct.
 * @str: The string representing the alias to be unset.
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
    ret = delete_node_at_index(&(info->alias), get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
    *p = c;
    return (ret);
}

/**
 * set_alias - Set an alias to a string.
 * @info: Parameter struct.
 * @str: The string representing the alias.
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
    return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Print an alias string.
 * @node: The alias node to be printed.
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
 * _myalias - Mimic the alias builtin (man alias).
 * @info: Structure containing potential arguments.
 *
 * If no arguments are provided, print all aliases with their corresponding values.
 * If arguments are provided in the format 'alias=value', set the alias accordingly.
 *
 * Return: Always 0.
 */
int _myalias(info_t *info)
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
            print_alias(node_starts_with(info->alias, info->argv[i], '='));
    }

    return (0);
}
