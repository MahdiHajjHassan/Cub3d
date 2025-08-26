/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:05:15 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 20:05:15 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d_bonus.h"
#include <math.h>

/* Declare check_collision implemented in input_bonus_helpers.c */
bool	check_collision(t_game *g, double new_x, double new_y);

static double	buf_sign(double v, double b)
{
	if (v > 0)
		return (b);
	return (-b);
}

static void	move_axis(t_game *g, double nx, double ny, double buf)
{
	double	off;

	off = buf_sign(nx - g->pos_x, buf);
	if (!check_collision(g, nx + off, g->pos_y))
		g->pos_x = nx;
	off = buf_sign(ny - g->pos_y, buf);
	if (!check_collision(g, g->pos_x, ny + off))
		g->pos_y = ny;
}

void	apply_movement_bonus(t_game *g)
{
	double	nx;
	double	ny;
	double	buf;
	double	rot;
	double	old_dir_x;
	double	old_plane_x;

	if (!g || g->closing)
		return ;
	buf = 0.1;
	if (g->keys.w)
	{
		nx = g->pos_x + g->dir_x * g->move_speed;
		ny = g->pos_y + g->dir_y * g->move_speed;
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.s)
	{
		nx = g->pos_x - g->dir_x * g->move_speed;
		ny = g->pos_y - g->dir_y * g->move_speed;
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.a)
	{
		nx = g->pos_x - g->plane_x * g->move_speed;
		ny = g->pos_y - g->plane_y * g->move_speed;
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.d)
	{
		nx = g->pos_x + g->plane_x * g->move_speed;
		ny = g->pos_y + g->plane_y * g->move_speed;
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.left || g->keys.right)
	{
		rot = g->rot_speed;
		if (g->keys.right)
			rot = -rot;
		old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
}

