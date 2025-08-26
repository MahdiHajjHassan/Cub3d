/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:12:30 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 20:12:30 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d_bonus.h"

bool	check_collision(t_game *g, double new_x, double new_y)
{
	int	map_x;
	int	map_y;
	char	cell;
	int	i;

	if (!g || !g->map)
		return (true);
	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_y < 0)
		return (true);
	if ((size_t)map_x >= g->map->width || (size_t)map_y >= g->map->height)
		return (true);
	cell = g->map->grid[map_y][map_x];
	if (cell == '1')
		return (true);
	if (cell == 'D')
	{
		i = 0;
		while (i < g->map->door_count)
		{
			if (g->map->doors[i].x == map_x && g->map->doors[i].y == map_y)
			{
				if (!g->map->doors[i].is_open)
					return (true);
				break ;
			}
			i++;
		}
	}
	return (false);
}

