/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_strafe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:05:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:39:40 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

static void	handle_left_strafe(t_game *g, double buf)
{
	double	nx;
	double	ny;
	double	speed_multiplier;

	nx = 0.0;
	ny = 0.0;
	if (g->keys.w || g->keys.s)
		speed_multiplier = g->move_speed * 0.52;
	else
		speed_multiplier = g->move_speed;
	nx = g->pos_x - g->plane_x * speed_multiplier;
	ny = g->pos_y - g->plane_y * speed_multiplier;
	move_axis(g, nx, ny, buf);
}

static void	handle_right_strafe(t_game *g, double buf)
{
	double	nx;
	double	ny;
	double	speed_multiplier;

	nx = 0.0;
	ny = 0.0;
	if (g->keys.w || g->keys.s)
		speed_multiplier = g->move_speed * 0.52;
	else
		speed_multiplier = g->move_speed;
	nx = g->pos_x + g->plane_x * speed_multiplier;
	ny = g->pos_y + g->plane_y * speed_multiplier;
	move_axis(g, nx, ny, buf);
}

void	handle_strafe_movement(t_game *g)
{
	double	buf;

	buf = 0.1;
	if (g->keys.a)
		handle_left_strafe(g, buf);
	if (g->keys.d)
		handle_right_strafe(g, buf);
}
