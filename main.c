#include "shell.h"

/**
 * my_info - program information
 *
 * @ac: number of arguments
 * @av: array of arguments
 *
 * Return: 0 on success, 1 on error
 */
int my_main(int ac, char **av)
{
	program_info_t my_info = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Unable to open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		my_info.readfd = fd;
	}
	populate_env_list(&my_info);
	read_history(&my_info);
	hsh(&my_info, av);
	return (EXIT_SUCCESS);
}
