/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:46:18 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 13:57:07 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	check_door_collision(t_game *g, int map_x, int map_y)
{
	int	i;

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
	return (false);
}

bool	check_collision(t_game *g, double new_x, double new_y)
{
	int		map_x;
	int		map_y;
	char	cell;

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
		return (check_door_collision(g, map_x, map_y));
	return (false);
}
