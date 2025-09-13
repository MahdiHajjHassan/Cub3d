/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
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

int	frame_init(t_game *g)
{
	g->frame.img = mlx_new_image(g->mlx, WIN_W, WIN_H);
	if (!g->frame.img)
		return (error_msg("mlx: image failed"));
	g->frame.data = mlx_get_data_addr(g->frame.img, &g->frame.bpp,
			&g->frame.line_len, &g->frame.endian);
	if (!g->frame.data)
	{
		mlx_destroy_image(g->mlx, g->frame.img);
		g->frame.img = NULL;
		return (error_msg("mlx: data addr failed"));
	}
	g->frame.width = WIN_W;
	g->frame.height = WIN_H;
	return (0);
}

void	frame_destroy(t_game *g)
{
	if (g->mlx && g->frame.img)
		mlx_destroy_image(g->mlx, g->frame.img);
	g->frame = (t_img){0};
}
