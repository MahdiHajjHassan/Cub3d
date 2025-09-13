/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_pixel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:00:00 by cub3d             #+#    #+#             */
/*   Updated: 2025/09/13 18:57:12 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/render.h"

static void	calculate_texture_y(t_sprite_draw_data data,
	t_sprite_pixel_data *pixel, t_img *tex)
{
	int	d;

	d = (pixel->y) * 256 - WIN_H * 128 + data.sprite_height * 128;
	pixel->tex_y = ((d * tex->height) / data.sprite_height) / 256;
	if (pixel->tex_y < 0)
		pixel->tex_y = 0;
	if (pixel->tex_y >= tex->height)
		pixel->tex_y = tex->height - 1;
}

static void	copy_pixel_data(char *tex_pixel, char *frame_pixel)
{
	frame_pixel[0] = tex_pixel[0];
	frame_pixel[1] = tex_pixel[1];
	frame_pixel[2] = tex_pixel[2];
	frame_pixel[3] = 0;
}

void	draw_sprite_pixel(t_game *g, t_img *tex,
	t_sprite_draw_data data, t_sprite_pixel_data *pixel)
{
	int		tex_bpp;
	int		frame_bpp;
	char	*tex_pixel;
	char	*frame_pixel;

	calculate_texture_y(data, pixel, tex);
	tex_bpp = tex->bpp / 8;
	frame_bpp = g->frame.bpp / 8;
	if (tex_bpp >= 4 && frame_bpp >= 4)
	{
		tex_pixel = tex->data + pixel->tex_y * tex->line_len
			+ pixel->tex_x * tex_bpp;
		frame_pixel = g->frame.data + pixel->y * g->frame.line_len
			+ data.stripe * frame_bpp;
		if (!check_transparency(tex_pixel))
			copy_pixel_data(tex_pixel, frame_pixel);
	}
}
