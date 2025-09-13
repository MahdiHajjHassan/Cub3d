/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:00:00 by cub3d             #+#    #+#             */
/*   Updated: 2025/09/13 23:28:50 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/render.h"

static void	calculate_sprite_transform(t_game *g, t_sprite *sprite,
	double *transform_x, double *transform_y)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;

	sprite_x = sprite->x - g->pos_x;
	sprite_y = sprite->y - g->pos_y;
	inv_det = 1.0 / (g->plane_x * g->dir_y - g->dir_x * g->plane_y);
	*transform_x = inv_det * (g->dir_y * sprite_x - g->dir_x * sprite_y);
	*transform_y = inv_det * (-g->plane_y * sprite_x + g->plane_x * sprite_y);
}

static void	calculate_sprite_dimensions(double transform_y,
	int *sprite_height, int *sprite_width)
{
	*sprite_height = iabs((int)(WIN_H / transform_y));
	*sprite_width = iabs((int)(WIN_H / transform_y));
}

static void	calculate_draw_bounds(int sprite_height, int sprite_screen_x,
	int sprite_width, t_sprite_bounds *bounds)
{
	bounds->draw_start_y = -sprite_height / 2 + WIN_H / 2;
	if (bounds->draw_start_y < 0)
		bounds->draw_start_y = 0;
	bounds->draw_end_y = sprite_height / 2 + WIN_H / 2;
	if (bounds->draw_end_y >= WIN_H)
		bounds->draw_end_y = WIN_H - 1;
	bounds->draw_start_x = -sprite_width / 2 + sprite_screen_x;
	if (bounds->draw_start_x < 0)
		bounds->draw_start_x = 0;
	bounds->draw_end_x = sprite_width / 2 + sprite_screen_x;
	if (bounds->draw_end_x >= WIN_W)
		bounds->draw_end_x = WIN_W - 1;
}

static void	setup_sprite_data(double transform_x, double transform_y,
	t_sprite_draw_data *data)
{
	data->sprite_screen_x = (int)((WIN_W / 2) * (1 + transform_x
				/ transform_y));
	calculate_sprite_dimensions(transform_y, &data->sprite_height,
		&data->sprite_width);
}

void	render_single_sprite(t_game *g, t_sprite *sprite)
{
	double				transform_x;
	double				transform_y;
	t_sprite_bounds		bounds;
	t_sprite_draw_data	data;
	t_sprite_pixel_data	pixel;

	calculate_sprite_transform(g, sprite, &transform_x, &transform_y);
	if (transform_y <= 0)
		return ;
	setup_sprite_data(transform_x, transform_y, &data);
	calculate_draw_bounds(data.sprite_height, data.sprite_screen_x,
		data.sprite_width, &bounds);
	data.draw_start_y = bounds.draw_start_y;
	data.draw_end_y = bounds.draw_end_y;
	data.stripe = bounds.draw_start_x;
	while (data.stripe < bounds.draw_end_x)
	{
		draw_sprite_column(g, sprite, data, &pixel);
		data.stripe++;
	}
}
