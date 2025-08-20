// Error reporting helper: prints mandatory prefix and a message to stderr.
#include "cub3d.h"

int	error_msg(const char *msg)
{
	if (msg)
	{
		write(2, "Error\n", 6);
		while (*msg)
			write(2, msg++, 1);
		write(2, "\n", 1);
	}
	return (1);
}


