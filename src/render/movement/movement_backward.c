/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_backward.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:39:40 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

static void	calculate_backward_speed(t_game *g, double *nx, double *ny)
{
	double	speed_multiplier;

	if (g->keys.a || g->keys.d)
		speed_multiplier = g->move_speed * 0.52;
	else
		speed_multiplier = g->move_speed;
	*nx = g->pos_x - g->dir_x * speed_multiplier;
	*ny = g->pos_y - g->dir_y * speed_multiplier;
}

void	handle_backward_movement(t_game *g)
{
	double	nx;
	double	ny;
	double	buf;

	if (!g->keys.s)
		return ;
	nx = 0.0;
	ny = 0.0;
	buf = 0.1;
	calculate_backward_speed(g, &nx, &ny);
	move_axis(g, nx, ny, buf);
}
