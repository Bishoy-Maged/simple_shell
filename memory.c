#include "shell.h"

/**
 * safe_free - safely frees a pointer and NULLs the address
 * @ptr: pointer to the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
