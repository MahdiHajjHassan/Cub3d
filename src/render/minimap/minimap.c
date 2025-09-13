/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:39:12 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:18:48 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

int	init_minimap(t_game *g)
{
	int	max_dim;
	int	fit_scale;

	g->minimap.size = MINIMAP_SIZE;
	g->minimap.margin = MINIMAP_MARGIN;
	if (g->map->width > g->map->height)
		max_dim = (int)g->map->width;
	else
		max_dim = (int)g->map->height;
	if (max_dim <= 0)
		fit_scale = MINIMAP_SCALE;
	else
		fit_scale = (g->minimap.size - 2) / max_dim;
	if (fit_scale <= 0)
		fit_scale = 1;
	g->minimap.scale = fit_scale;
	return (setup_minimap_image(g));
}

int	setup_minimap_image(t_game *g)
{
	g->minimap.offset_x = (g->minimap.size - (int)g->map->width
			* g->minimap.scale) / 2;
	g->minimap.offset_y = (g->minimap.size - (int)g->map->height
			* g->minimap.scale) / 2;
	if (g->minimap.offset_x < 0)
		g->minimap.offset_x = 0;
	if (g->minimap.offset_y < 0)
		g->minimap.offset_y = 0;
	g->minimap.img.img = mlx_new_image(g->mlx, g->minimap.size,
			g->minimap.size);
	if (!g->minimap.img.img)
		return (error_msg("minimap: image creation failed"));
	g->minimap.img.data = mlx_get_data_addr(g->minimap.img.img,
			&g->minimap.img.bpp, &g->minimap.img.line_len,
			&g->minimap.img.endian);
	if (!g->minimap.img.data)
	{
		mlx_destroy_image(g->mlx, g->minimap.img.img);
		return (error_msg("minimap: data addr failed"));
	}
	g->minimap.img.width = g->minimap.size;
	g->minimap.img.height = g->minimap.size;
	return (0);
}

void	destroy_minimap(t_game *g)
{
	if (g->mlx && g->minimap.img.img)
		mlx_destroy_image(g->mlx, g->minimap.img.img);
	g->minimap.img = (t_img){0};
}

void	draw_minimap_pixel(t_img *img, t_pixel_data pixel)
{
	int		bpp;
	char	*p;

	if (!img || !img->data || pixel.x < 0 || pixel.y < 0)
		return ;
	if (pixel.x >= img->width || pixel.y >= img->height)
		return ;
	bpp = img->bpp / 8;
	if (bpp < 4)
		return ;
	p = img->data + pixel.y * img->line_len + pixel.x * bpp;
	p[0] = (char)(pixel.color & 0xFF);
	p[1] = (char)((pixel.color >> 8) & 0xFF);
	p[2] = (char)((pixel.color >> 16) & 0xFF);
	p[3] = 0;
}

void	draw_minimap_rect(t_img *img, t_minimap_rect_data rect)
{
	int	i;
	int	j;

	i = 0;
	while (i < rect.size)
	{
		j = 0;
		while (j < rect.size)
		{
			draw_minimap_pixel(img, (t_pixel_data){rect.x + j, rect.y + i,
				rect.color});
			j++;
		}
		i++;
	}
}
