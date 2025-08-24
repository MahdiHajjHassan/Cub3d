/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:35:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 18:35:00 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#if defined(BONUS)
# include "cub3d_bonus.h"
#else
# include "cub3d.h"
#endif

int	main(int argc, char **argv)
{
	t_config	cfg;

	if (argc != 2)
	{
		write(2, "Error\nusage: ./cub3d file.cub\n", 29);
		return (1);
	}
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
