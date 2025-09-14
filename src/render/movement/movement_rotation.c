/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_rotation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:10:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:03:36 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <math.h>

static void	rotate_direction_vector(t_game *g, double rot)
{
	double	old_dir_x;

	old_dir_x = 0.0;
	old_dir_x = g->dir_x;
	g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
	g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
}

static void	rotate_plane_vector(t_game *g, double rot)
{
	double	old_plane_x;

	old_plane_x = 0.0;
	old_plane_x = g->plane_x;
	g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
	g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
}

void	handle_rotation_movement(t_game *g)
{
	double	rot;
	double	time_adjusted_rot;

	if (!g->keys.left && !g->keys.right)
		return ;
	rot = 0.0;
	rot = g->rot_speed;
	if (g->keys.left)
		rot = -rot;
	time_adjusted_rot = rot * g->frame_time;
	rotate_direction_vector(g, time_adjusted_rot);
	rotate_plane_vector(g, time_adjusted_rot);
}
