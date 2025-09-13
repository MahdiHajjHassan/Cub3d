/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:50:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:39:40 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <math.h>

static double	buf_sign(double v, double b)
{
	if (v > 0)
		return (b);
	return (-b);
}

void	move_axis(t_game *g, double nx, double ny, double buf)
{
	double	off;

	off = 0.0;
	off = buf_sign(nx - g->pos_x, buf);
	if (!check_collision(g, nx + off, g->pos_y))
		g->pos_x = nx;
	off = buf_sign(ny - g->pos_y, buf);
	if (!check_collision(g, g->pos_x, ny + off))
		g->pos_y = ny;
}

void	apply_movement(t_game *g)
{
	if (!g || g->closing)
		return ;
	handle_forward_movement(g);
	handle_backward_movement(g);
	handle_strafe_movement(g);
	handle_rotation_movement(g);
}
