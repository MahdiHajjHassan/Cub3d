/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_forward.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:55:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:03:36 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

static void	calculate_forward_speed(t_game *g, double *nx, double *ny)
{
	double	speed_multiplier;
	double	time_adjusted_speed;

	if (g->keys.a || g->keys.d)
	{
		if (g->keys.space)
			speed_multiplier = g->move_speed * 1.04;
		else
			speed_multiplier = g->move_speed * 0.52;
	}
	else
	{
		if (g->keys.space)
			speed_multiplier = g->move_speed * 2;
		else
			speed_multiplier = g->move_speed;
	}
	time_adjusted_speed = speed_multiplier * g->frame_time;
	*nx = g->pos_x + g->dir_x * time_adjusted_speed;
	*ny = g->pos_y + g->dir_y * time_adjusted_speed;
}

void	handle_forward_movement(t_game *g)
{
	double	nx;
	double	ny;
	double	buf;

	if (!g->keys.w)
		return ;
	nx = 0.0;
	ny = 0.0;
	buf = 0.1;
	calculate_forward_speed(g, &nx, &ny);
	move_axis(g, nx, ny, buf);
}
