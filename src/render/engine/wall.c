/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
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

static void	calculate_wall_distance(t_game *g, t_ray_data *ray,
	t_wall_data *wall)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x != 0)
			wall->perp_wall_dist = (ray->map_x - g->pos_x
					+ (1 - ray->step_x) / 2.0) / ray->ray_dir_x;
		else
			wall->perp_wall_dist = (ray->map_x - g->pos_x
					+ (1 - ray->step_x) / 2.0) / 1e-6;
	}
	else
	{
		if (ray->ray_dir_y != 0)
			wall->perp_wall_dist = (ray->map_y - g->pos_y
					+ (1 - ray->step_y) / 2.0) / ray->ray_dir_y;
		else
			wall->perp_wall_dist = (ray->map_y - g->pos_y
					+ (1 - ray->step_y) / 2.0) / 1e-6;
	}
	if (wall->perp_wall_dist < 1e-6)
		wall->perp_wall_dist = 1e-6;
}

void	calculate_wall(t_game *g, t_ray_data *ray, t_wall_data *wall)
{
	calculate_wall_distance(g, ray, wall);
	wall->line_height = (int)(WIN_H / wall->perp_wall_dist);
	wall->draw_start = -wall->line_height / 2 + WIN_H / 2;
	wall->draw_end = wall->line_height / 2 + WIN_H / 2;
	if (wall->draw_start < 0)
		wall->draw_start = 0;
	if (wall->draw_end > WIN_H)
		wall->draw_end = WIN_H;
}

void	setup_wall_texture(t_game *g, t_ray_data *ray, t_wall_data *wall)
{
	if (ray->side == 0)
		wall->wall_x = g->pos_y + wall->perp_wall_dist * ray->ray_dir_y;
	else
		wall->wall_x = g->pos_x + wall->perp_wall_dist * ray->ray_dir_x;
	wall->wall_x -= floor(wall->wall_x);
	if (ray->hit_door)
		wall->tex_id = TEX_DOOR;
	else if (ray->side == 0 && ray->ray_dir_x > 0)
		wall->tex_id = TEX_WE;
	else if (ray->side == 0 && ray->ray_dir_x < 0)
		wall->tex_id = TEX_EA;
	else if (ray->side == 1 && ray->ray_dir_y > 0)
		wall->tex_id = TEX_NO;
	else
		wall->tex_id = TEX_SO;
}
