/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 23:43:38 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	draw_player_rect(t_game *g, int map_x, int map_y)
{
	int	player_size;
	int	color;

	player_size = 3;
	color = 0xFF0000;
	draw_minimap_rect(&g->minimap.img, (t_minimap_rect_data){map_x, map_y,
		player_size, color});
}

void	draw_player_on_minimap(t_game *g)
{
	int	map_x;
	int	map_y;

	map_x = g->minimap.offset_x + (int)(g->pos_x * g->minimap.scale) - 1;
	map_y = g->minimap.offset_y + (int)(g->pos_y * g->minimap.scale) - 1;
	draw_player_rect(g, map_x, map_y);
	draw_line_on_minimap(g, map_x, map_y);
}

static int	calculate_line_steps(int dx, int dy)
{
	int	abs_dx;
	int	abs_dy;

	if (dx < 0)
		abs_dx = -dx;
	else
		abs_dx = dx;
	if (dy < 0)
		abs_dy = -dy;
	else
		abs_dy = dy;
	if (abs_dx > abs_dy)
		return (abs_dx);
	return (abs_dy);
}

static void	calculate_line_endpoints(t_game *g, int start_x, int start_y,
	t_line_data *line_data)
{
	int	center_x;
	int	center_y;
	int	end_x;
	int	end_y;

	center_x = start_x + 1;
	center_y = start_y + 1;
	end_x = center_x + (int)(g->dir_x * 10);
	end_y = center_y + (int)(g->dir_y * 10);
	line_data->start_x = center_x;
	line_data->start_y = center_y;
	line_data->dx = end_x - center_x;
	line_data->dy = end_y - center_y;
	line_data->steps = calculate_line_steps(line_data->dx, line_data->dy);
}

void	draw_line_on_minimap(t_game *g, int start_x, int start_y)
{
	t_line_data	line_data;

	calculate_line_endpoints(g, start_x, start_y, &line_data);
	draw_direction_line(g, line_data);
}
