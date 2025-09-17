/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:40:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/17 14:41:24 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

void	clear_minimap_background(t_game *g)
{
	int		total_bytes;
	char	*data_ptr;

	if (!g || !g->minimap.img.data)
		return ;
	total_bytes = g->minimap.img.line_len * g->minimap.size;
	data_ptr = g->minimap.img.data;
	ft_memset(data_ptr, 0, total_bytes);
}

static int	get_cell_color(char cell, bool is_door_open)
{
	if (cell == '1')
		return (0xFFFFFF);
	else if (cell == 'D')
	{
		if (is_door_open)
			return (0x00FF00);
		return (0x8B4513);
	}
	else if (cell == '0' || cell == ' ')
		return (0x404040);
	return (0x000000);
}

static bool	check_door_state(t_game *g, int x, int y)
{
	int	i;

	i = 0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].x == x && g->map->doors[i].y == y)
			return (g->map->doors[i].is_open);
		i++;
	}
	return (false);
}

void	draw_map_cell(t_game *g, int x, int y)
{
	char	cell;
	int		color;
	int		map_x;
	int		map_y;

	cell = g->map->grid[y][x];
	color = get_cell_color(cell, check_door_state(g, x, y));
	map_x = g->minimap.offset_x + x * g->minimap.scale;
	map_y = g->minimap.offset_y + y * g->minimap.scale;
	draw_minimap_rect(&g->minimap.img, (t_minimap_rect_data){map_x, map_y,
		g->minimap.scale, color});
}
