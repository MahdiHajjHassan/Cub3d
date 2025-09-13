/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_column.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:00:00 by cub3d             #+#    #+#             */
/*   Updated: 2025/09/13 23:28:50 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/render.h"

static int	get_texture_data(t_game *g, t_sprite *sprite, t_img **tex)
{
	int	tex_id;

	tex_id = SPRITE_TEX_START + sprite->texture_id * MAX_SPRITE_FRAMES
		+ sprite->current_frame;
	*tex = &g->tex[tex_id];
	if (!(*tex)->data || tex_id >= TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
		return (0);
	return (tex_id);
}

static void	calculate_transform_depth(t_game *g, t_sprite *sprite,
	double *transform_y)
{
	double	sprite_x_world;
	double	sprite_y_world;
	double	inv_det;

	sprite_x_world = sprite->x - g->pos_x;
	sprite_y_world = sprite->y - g->pos_y;
	inv_det = 1.0 / (g->plane_x * g->dir_y - g->dir_x * g->plane_y);
	*transform_y = inv_det * (-g->plane_y * sprite_x_world
			+ g->plane_x * sprite_y_world);
}

static void	calculate_texture_coordinates(t_sprite_draw_data data,
	t_img *tex, t_sprite_pixel_data *pixel)
{
	pixel->tex_x = (int)(256 * (data.stripe - (-data.sprite_width / 2
					+ data.sprite_screen_x)) * tex->width
			/ data.sprite_width) / 256;
	if (pixel->tex_x < 0)
		pixel->tex_x = 0;
	if (pixel->tex_x >= tex->width)
		pixel->tex_x = tex->width - 1;
}

static void	render_sprite_pixels(t_game *g, t_img *tex,
	t_sprite_draw_data data, t_sprite_pixel_data *pixel)
{
	pixel->y = data.draw_start_y;
	while (pixel->y < data.draw_end_y)
	{
		draw_sprite_pixel(g, tex, data, pixel);
		pixel->y++;
	}
}

void	draw_sprite_column(t_game *g, t_sprite *sprite,
	t_sprite_draw_data data, t_sprite_pixel_data *pixel)
{
	t_img	*tex;

	if (!get_texture_data(g, sprite, &tex))
		return ;
	calculate_texture_coordinates(data, tex, pixel);
	calculate_transform_depth(g, sprite, &pixel->transform_y);
	if (pixel->transform_y <= 0 || data.stripe < 0 || data.stripe >= WIN_W
		|| pixel->transform_y >= g->zbuffer[data.stripe])
		return ;
	render_sprite_pixels(g, tex, data, pixel);
}
