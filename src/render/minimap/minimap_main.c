/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 23:43:38 by hsharaf-         ###   ########.fr       */
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

static void	display_minimap_to_window(t_game *g)
{
	if (!g->closing && g->mlx && g->win)
		mlx_put_image_to_window(g->mlx, g->win, g->minimap.img.img,
			g->minimap.margin, g->minimap.margin);
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
