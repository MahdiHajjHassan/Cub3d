/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
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

void	init_ray(t_game *g, t_ray_data *ray, int x)
{
	ray->camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->ray_dir_x = g->dir_x + g->plane_x * ray->camera_x;
	ray->ray_dir_y = g->dir_y + g->plane_y * ray->camera_x;
	ray->map_x = (int)g->pos_x;
	ray->map_y = (int)g->pos_y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	ray->hit_door = false;
}

void	setup_ray_steps(t_game *g, t_ray_data *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (g->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - g->pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (g->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - g->pos_y) * ray->delta_dist_y;
	}
}

static bool	check_door_collision(t_game *g, t_ray_data *ray, t_door_data *door)
{
	door->i = 0;
	door->door_is_open = false;
	while (door->i < g->map->door_count)
	{
		if (g->map->doors[door->i].x == ray->map_x
			&& g->map->doors[door->i].y == ray->map_y)
		{
			door->door_is_open = g->map->doors[door->i].is_open;
			break ;
		}
		door->i++;
	}
	if (!door->door_is_open)
	{
		ray->hit_door = true;
		return (true);
	}
	return (false);
}

void	perform_dda(t_game *g, t_ray_data *ray, t_door_data *door)
{
	while (1)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_y < 0
			|| ray->map_x >= (int)g->map->width
			|| ray->map_y >= (int)g->map->height)
			break ;
		door->cell = g->map->grid[ray->map_y][ray->map_x];
		if (door->cell == '1')
			break ;
		else if (door->cell == 'D' && check_door_collision(g, ray, door))
			break ;
	}
}
