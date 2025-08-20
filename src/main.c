#include "cub3d.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Error\nusage: ./cub3d file.cub\n", 29);
		return (1);
	}
	t_config cfg;
	if (parse_cub_file(argv[1], &cfg) != 0)
		return (1);
	if (run_game(&cfg) != 0)
	{
		free_config(&cfg);
		return (1);
	}
	free_config(&cfg);
	return (0);
}


