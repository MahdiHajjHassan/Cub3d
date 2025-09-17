/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/17 14:48:07 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	draw_map_grid(t_game *g)
{
	int	x;
	int	y;

	y = 0;
	while (y < (int)g->map->height)
	{
		x = 0;
		while (x < (int)g->map->width)
		{
			draw_map_cell(g, x, y);
			x++;
		}
		y++;
	}
}

static void	draw_sprites_on_minimap(t_game *g)
{
	int	i;
	int	sprite_x;
	int	sprite_y;

	i = 0;
	while (i < g->map->sprite_count)
	{
		sprite_x = g->minimap.offset_x
			+ (int)(g->map->sprites[i].x * g->minimap.scale) - 1;
		sprite_y = g->minimap.offset_y
			+ (int)(g->map->sprites[i].y * g->minimap.scale) - 1;
		draw_minimap_rect(&g->minimap.img, (t_minimap_rect_data){sprite_x,
			sprite_y, 2, 0x0000FF});
		i++;
	}
}

static void	copy_minimap_pixel(t_game *g, int src_x, int src_y)
{
	int		dst_x;
	int		dst_y;
	char	*src_pixel;
	char	*dst_pixel;

	dst_x = src_x + g->minimap.margin;
	dst_y = src_y + g->minimap.margin;
	if (dst_x < WIN_W && dst_y < WIN_H
		&& g->minimap.img.bpp >= 32 && g->frame.bpp >= 32)
	{
		src_pixel = g->minimap.img.data + src_y
			* g->minimap.img.line_len + src_x * 4;
		dst_pixel = g->frame.data + dst_y
			* g->frame.line_len + dst_x * 4;
		dst_pixel[0] = src_pixel[0];
		dst_pixel[1] = src_pixel[1];
		dst_pixel[2] = src_pixel[2];
		dst_pixel[3] = 0;
	}
}

static void	display_minimap_to_window(t_game *g)
{
	int	src_x;
	int	src_y;

	if (!g->closing && g->mlx && g->win
		&& g->minimap.img.data && g->frame.data)
	{
		src_y = 0;
		while (src_y < g->minimap.size)
		{
			src_x = 0;
			while (src_x < g->minimap.size)
			{
				copy_minimap_pixel(g, src_x, src_y);
				src_x++;
			}
			src_y++;
		}
	}
}

void	render_minimap(t_game *g)
{
	if (!g || g->closing)
		return ;
	clear_minimap_background(g);
	draw_map_grid(g);
	draw_sprites_on_minimap(g);
	draw_player_on_minimap(g);
	display_minimap_to_window(g);
}
