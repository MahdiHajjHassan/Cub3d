/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:46:18 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/17 14:05:52 by hsharaf-         ###   ########.fr       */
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

static bool	check_wall_collision(t_game *g, int map_x, int map_y)
{
	char	cell;

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

bool	check_collision(t_game *g, double new_x, double new_y)
{
	double	margin;
	int		corners[4][2];
	int		i;

	if (!g || !g->map)
		return (true);
	margin = 0.15;
	corners[0][0] = (int)(new_x - margin);
	corners[0][1] = (int)(new_y - margin);
	corners[1][0] = (int)(new_x + margin);
	corners[1][1] = (int)(new_y - margin);
	corners[2][0] = (int)(new_x - margin);
	corners[2][1] = (int)(new_y + margin);
	corners[3][0] = (int)(new_x + margin);
	corners[3][1] = (int)(new_y + margin);
	i = 0;
	while (i < 4)
	{
		if (check_wall_collision(g, corners[i][0], corners[i][1]))
			return (true);
		i++;
	}
	return (false);
}
