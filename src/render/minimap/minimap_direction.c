/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_direction.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:35:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 23:43:38 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	draw_line_pixel(t_img *img, int x, int y, int color)
{
	t_pixel_data	pixel_data;

	pixel_data.x = x;
	pixel_data.y = y;
	pixel_data.color = color;
	draw_minimap_pixel(img, pixel_data);
}

static void	calculate_line_position(t_line_data *line, int i, int *x, int *y)
{
	*x = line->start_x + (line->dx * i) / line->steps;
	*y = line->start_y + (line->dy * i) / line->steps;
}

void	draw_direction_line(t_game *g, t_line_data line)
{
	int	i;
	int	x;
	int	y;
	int	color;

	i = 0;
	color = 0xFF0000;
	while (i <= line.steps)
	{
		calculate_line_position(&line, i, &x, &y);
		draw_line_pixel(&g->minimap.img, x, y, color);
		i++;
	}
}
