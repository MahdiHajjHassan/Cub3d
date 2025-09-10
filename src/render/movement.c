/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:44:42 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/10 16:45:29 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/* Declare check_collision implemented in input_helpers.c */
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

void	apply_movement(t_game *g)
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
		if(g->keys.a || g->keys.d)
		{
			if(g->keys.space)
			{
				nx = g->pos_x + g->dir_x * (g->move_speed*1.04);
				ny = g->pos_y + g->dir_y * (g->move_speed*1.04);
			}
			else
			{
				nx = g->pos_x + g->dir_x * (g->move_speed*0.52);
				ny = g->pos_y + g->dir_y * (g->move_speed*0.52);
			}
		}
		else
		{
			if(g->keys.space)
			{
				ny = g->pos_y + g->dir_y * (g->move_speed*2);
				nx = g->pos_x + g->dir_x * (g->move_speed*2);
			}
			else
			{
				ny = g->pos_y + g->dir_y * g->move_speed;
				nx = g->pos_x + g->dir_x * g->move_speed;
			}
		}
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.s)
	{
		if(g->keys.a || g->keys.d)
		{
			nx = g->pos_x - g->dir_x * (g->move_speed*0.52);
			ny = g->pos_y - g->dir_y * (g->move_speed*0.52);
		}
		else
		{
			nx = g->pos_x - g->dir_x * g->move_speed;
			ny = g->pos_y - g->dir_y * g->move_speed;
		}
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.a)
	{
		if(g->keys.w || g->keys.s)
		{
			nx = g->pos_x - g->plane_x * (g->move_speed*0.52);
			ny = g->pos_y - g->plane_y * (g->move_speed*0.52);
		}
		else
		{
			nx = g->pos_x - g->plane_x * g->move_speed;
			ny = g->pos_y - g->plane_y * g->move_speed;
		}
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.d)
	{
		if(g->keys.w || g->keys.s)
		{
			nx = g->pos_x + g->plane_x * (g->move_speed*0.52);
			ny = g->pos_y + g->plane_y * (g->move_speed*0.52);
		}
		else
		{
			nx = g->pos_x + g->plane_x * g->move_speed;
			ny = g->pos_y + g->plane_y * g->move_speed;
		}
		move_axis(g, nx, ny, buf);
	}
	if (g->keys.left || g->keys.right)
	{
		rot = g->rot_speed;
		if (g->keys.left)
			rot = -rot;
		old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
}

