/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:46:10 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 17:38:40 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	render_pixel_column(t_game *g, int x)
{
	t_ray_data	ray;
	t_wall_data	wall;
	t_door_data	door;

	init_ray(g, &ray, x);
	setup_ray_steps(g, &ray);
	perform_dda(g, &ray, &door);
	calculate_wall(g, &ray, &wall);
	setup_wall_texture(g, &ray, &wall);
	g->zbuffer[x] = wall.perp_wall_dist;
	draw_vertical_stripe(g, (t_stripe_data){x, wall.tex_id,
		wall.draw_start, wall.draw_end, wall.line_height, wall.wall_x});
}

void	render_frame(t_game *g)
{
	int			x;

	if (!g || g->closing)
		return ;
	fill_rect(&g->frame, (t_rect_data){0, 0, WIN_W, WIN_H / 2,
		g->ceil_color});
	fill_rect(&g->frame, (t_rect_data){0, WIN_H / 2, WIN_W, WIN_H,
		g->floor_color});
	x = 0;
	while (x < WIN_W)
	{
		render_pixel_column(g, x);
		x++;
	}
	render_sprites(g);
	if (!g->closing && g->mlx && g->win)
		mlx_put_image_to_window(g->mlx, g->win, g->frame.img, 0, 0);
	render_minimap(g);
}
