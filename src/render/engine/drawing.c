/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 17:28:39 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	fill_row(t_img *img, t_rect_data rect, int y, int bpp)
{
	int		x;
	char	*p;

	x = rect.x0;
	while (x < rect.x1)
	{
		p = img->data + y * img->line_len + x * bpp;
		p[0] = (char)(rect.color & 0xFF);
		p[1] = (char)((rect.color >> 8) & 0xFF);
		p[2] = (char)((rect.color >> 16) & 0xFF);
		p[3] = 0;
		x++;
	}
}

void	fill_rect(t_img *img, t_rect_data rect)
{
	int		y;
	int		bpp;

	if (!img || !img->data)
		return ;
	bpp = img->bpp / 8;
	if (bpp < 4)
		return ;
	if (rect.x0 < 0)
		rect.x0 = 0;
	if (rect.y0 < 0)
		rect.y0 = 0;
	if (rect.x1 > img->width)
		rect.x1 = img->width;
	if (rect.y1 > img->height)
		rect.y1 = img->height;
	y = rect.y0;
	while (y < rect.y1)
	{
		fill_row(img, rect, y, bpp);
		y++;
	}
}

static void	draw_texture_column(t_game *g, t_texture_data *tex_data,
	t_stripe_data stripe)
{
	int		y;
	int		tex_y;
	char	*src;
	char	*dst;

	y = stripe.draw_start;
	while (y < stripe.draw_end)
	{
		if (tex_data->tx->height)
			tex_y = (int)tex_data->tex_pos % tex_data->tx->height;
		else
			tex_y = (int)tex_data->tex_pos % 1;
		tex_data->tex_pos += tex_data->step;
		src = tex_data->tx->data + tex_y * tex_data->tx->line_len
			+ tex_data->tex_x * tex_data->tbpp;
		dst = g->frame.data + y * g->frame.line_len
			+ stripe.x * tex_data->bpp;
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = 0;
		y++;
	}
}

void	draw_vertical_stripe(t_game *g, t_stripe_data stripe)
{
	t_texture_data	tex_data;

	tex_data.tx = &g->tex[stripe.tex_id];
	tex_data.bpp = g->frame.bpp / 8;
	tex_data.tbpp = tex_data.tx->bpp / 8;
	if (tex_data.tbpp < 4 || tex_data.bpp < 4)
		return ;
	tex_data.tex_x = (int)(stripe.wall_x * (double)tex_data.tx->width);
	if (tex_data.tex_x < 0)
		tex_data.tex_x = 0;
	if (tex_data.tex_x >= tex_data.tx->width)
		tex_data.tex_x = tex_data.tx->width - 1;
	tex_data.step = 1.0 * tex_data.tx->height / stripe.line_height;
	tex_data.tex_pos = (stripe.draw_start - WIN_H / 2
			+ stripe.line_height / 2) * tex_data.step;
	draw_texture_column(g, &tex_data, stripe);
}
