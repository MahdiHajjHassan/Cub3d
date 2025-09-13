/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_doors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:58 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 17:46:50 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_valid_door_position(t_game *g, int door_x, int door_y)
{
	if (door_x < 0 || door_y < 0)
		return (0);
	if ((size_t)door_x >= g->map->width || (size_t)door_y >= g->map->height)
		return (0);
	return (1);
}

static void	toggle_door_at_position(t_game *g, int door_x, int door_y)
{
	int	i;

	i = 0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].x == door_x && g->map->doors[i].y == door_y)
		{
			g->map->doors[i].is_open = !g->map->doors[i].is_open;
			g->map->doors[i].open_timer = 0.0;
			return ;
		}
		i++;
	}
}

void	handle_door_interaction(t_game *g)
{
	double	t;
	int		door_x;
	int		door_y;

	if (!g || g->closing)
		return ;
	t = 0.5;
	while (t <= 2.0)
	{
		door_x = (int)(g->pos_x + g->dir_x * t);
		door_y = (int)(g->pos_y + g->dir_y * t);
		if (!is_valid_door_position(g, door_x, door_y))
			break ;
		if (g->map->grid[door_y][door_x] == 'D')
		{
			toggle_door_at_position(g, door_x, door_y);
			return ;
		}
		t += 0.1;
	}
}

void	update_doors(t_game *g)
{
	int		i;
	double	limit;

	if (!g)
		return ;
	i = 0;
	limit = 3.0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].is_open)
		{
			g->map->doors[i].open_timer += g->frame_time;
			if (g->map->doors[i].open_timer > limit)
			{
				g->map->doors[i].is_open = 0;
				g->map->doors[i].open_timer = 0.0;
			}
		}
		i++;
	}
}
