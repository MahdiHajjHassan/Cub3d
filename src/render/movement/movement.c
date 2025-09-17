/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:50:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/17 14:13:44 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <math.h>

static void	calculate_forward_movement(t_game *g, double *forward_dx,
		double *forward_dy)
{
	double	speed_mult;
	double	time_speed;

	*forward_dx = 0.0;
	*forward_dy = 0.0;
	if (!(g->keys.w || g->keys.s))
		return ;
	if (g->keys.a || g->keys.d)
		speed_mult = g->move_speed * 0.52;
	else if (g->keys.w && g->keys.space)
		speed_mult = g->move_speed * 2;
	else
		speed_mult = g->move_speed;
	time_speed = speed_mult * g->frame_time;
	if (g->keys.w)
	{
		*forward_dx = g->dir_x * time_speed;
		*forward_dy = g->dir_y * time_speed;
	}
	if (g->keys.s)
	{
		*forward_dx -= g->dir_x * time_speed;
		*forward_dy -= g->dir_y * time_speed;
	}
}

static void	calculate_strafe_movement(t_game *g, double *strafe_dx,
		double *strafe_dy)
{
	double	speed_mult;
	double	time_speed;

	*strafe_dx = 0.0;
	*strafe_dy = 0.0;
	if (!(g->keys.a || g->keys.d))
		return ;
	if (g->keys.w || g->keys.s)
		speed_mult = g->move_speed * 0.52;
	else
		speed_mult = g->move_speed;
	time_speed = speed_mult * g->frame_time;
	if (g->keys.a)
	{
		*strafe_dx = -g->plane_x * time_speed;
		*strafe_dy = -g->plane_y * time_speed;
	}
	if (g->keys.d)
	{
		*strafe_dx += g->plane_x * time_speed;
		*strafe_dy += g->plane_y * time_speed;
	}
}

static void	calculate_total_movement(t_game *g, double *total_dx,
		double *total_dy)
{
	double	forward_dx;
	double	forward_dy;
	double	strafe_dx;
	double	strafe_dy;

	*total_dx = 0.0;
	*total_dy = 0.0;
	calculate_forward_movement(g, &forward_dx, &forward_dy);
	calculate_strafe_movement(g, &strafe_dx, &strafe_dy);
	*total_dx = forward_dx + strafe_dx;
	*total_dy = forward_dy + strafe_dy;
}

void	move_axis(t_game *g, double nx, double ny, double buf)
{
	if (!check_collision(g, nx, ny))
	{
		g->pos_x = nx;
		g->pos_y = ny;
		return ;
	}
	if (!check_collision(g, nx, g->pos_y))
		g->pos_x = nx;
	else if (!check_collision(g, g->pos_x, ny))
		g->pos_y = ny;
	(void)buf;
}

void	apply_movement(t_game *g)
{
	double	total_dx;
	double	total_dy;
	double	new_x;
	double	new_y;

	if (!g || g->closing)
		return ;
	calculate_total_movement(g, &total_dx, &total_dy);
	new_x = g->pos_x + total_dx;
	new_y = g->pos_y + total_dy;
	move_axis(g, new_x, new_y, 0.1);
	handle_rotation_movement(g);
}
