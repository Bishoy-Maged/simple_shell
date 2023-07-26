#include "shell.h"

/**
 * add_node - Adds a new node to the beginning of the list.
 * @head: A pointer to the head node pointer.
 * @str: The string to store in the new node.
 * @num: The node index used by history.
 *
 * Return: The address of the new head node, or NULL on failure.
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return (NULL);

	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);

	/* Initialize the new node */
	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}

	/* Update the list head */
	new_head->next = *head;
	*head = new_head;

	return (new_head);
}

/**
 * add_node_end - Adds a new node to the end of the list.
 * @head: A pointer to the head node pointer.
 * @str: The string to store in the new node.
 * @num: The node index used by history.
 *
 * Return: The address of the new end node, or NULL on failure.
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	/* Initialize the new node */
	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}

	if (*head)
	{
		/* Traverse the list to find the end node */
		node = *head;
		while (node->next)
			node = node->next;

		/* Append the new node to the end of the list */
		node->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * print_list_str - Prints only the string element of a list_t linked list.
 * @h: A pointer to the first node.
 *
 * Return: The size of the list.
 */
size_t print_list_str(const list_t *h)
{
	size_t size = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		size++;
	}

	return (size);
}

/**
 * delete_node_at_index - Deletes the node at the given index.
 * @head: A pointer to the head node pointer.
 * @index: The index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		/* Delete the head node */
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	/* Traverse the list to find the node at the given index */
	node = *head;
	while (node)
	{
		if (i == index)
		{
			/* Delete the node at the given index */
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}

	return (0);
}

/**
 * free_list - Frees all nodes of a list.
 * @head_ptr: A pointer to the head node pointer.
 *
 * Return: void
 */
void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;

		/* Free the memory allocated for the node */
		free(node->str);
		free(node);

		node = next_node;
	}

	*head_ptr = NULL;
}
