/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_bonus_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:35:30 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 19:35:30 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d_bonus.h"
#include "mlx.h"

static int	iabs_local(int v)
{
	if (v < 0)
		return (-v);
	return (v);
}

/* static removed intentionally: referenced from other units */
bool	check_collision(t_game *g, double new_x, double new_y)
{
	int	map_x;
	int	map_y;
	char	cell;
	int	i;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_y < 0 || map_x >= (int)g->map->width
		|| map_y >= (int)g->map->height)
		return (true);
	cell = g->map->grid[map_y][map_x];
	if (cell == '1')
		return (true);
	if (cell == 'D')
	{
		i = 0;
		while (i < g->map->door_count)
		{
			if (g->map->doors[i].x == map_x && g->map->doors[i].y == map_y)
				return (!g->map->doors[i].is_open);
			i++;
		}
	}
	return (false);
}

void	handle_door_interaction(t_game *g)
{
	double	check_dist;
	double	check_x;
	double	check_y;
	int		door_x;
	int		door_y;
	int		i;

	check_dist = 1.5;
	check_x = g->pos_x + g->dir_x * check_dist;
	check_y = g->pos_y + g->dir_y * check_dist;
	door_x = (int)check_x;
	door_y = (int)check_y;
	if (door_x < 0 || door_y < 0)
		return ;
	if ((size_t)door_x >= g->map->width || (size_t)door_y >= g->map->height)
		return ;
	if (g->map->grid[door_y][door_x] != 'D')
		return ;
	i = 0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].x == door_x && g->map->doors[i].y == door_y)
		{
			g->map->doors[i].is_open = !g->map->doors[i].is_open;
			g->map->doors[i].open_timer = 0.0;
			break ;
		}
		i++;
	}
}

void	update_doors(t_game *g)
{
	int		i;
	double	limit;

	i = 0;
	limit = 3.0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].is_open)
		{
			g->map->doors[i].open_timer += g->frame_time;
			if (g->map->doors[i].open_timer > limit)
			{
				g->map->doors[i].is_open = false;
				g->map->doors[i].open_timer = 0.0;
			}
		}
		i++;
	}
}

int	on_mouse_move(int x, int y, void *param)
{
	t_game	*g;
	int		dx;
	double	sensitivity;
	double	rot;
	double	old_dir_x;
	double	old_plane_x;

	g = (t_game *)param;
	if (!g || g->closing || !g->mlx || !g->win)
		return (0);
	if (x < 50 || x > WIN_W - 50 || y < 50 || y > WIN_H - 50)
	{
		if (g->mlx && g->win)
			mlx_mouse_move(g->mlx, g->win, WIN_W / 2, WIN_H / 2);
		g->mouse.last_x = WIN_W / 2;
		g->mouse.last_y = WIN_H / 2;
		g->mouse.x = WIN_W / 2;
		g->mouse.y = WIN_H / 2;
		g->mouse.captured = true;
		return (0);
	}
	if (!g->mouse.captured)
	{
		g->mouse.x = x;
		g->mouse.y = y;
		g->mouse.last_x = x;
		g->mouse.last_y = y;
		g->mouse.captured = true;
		return (0);
	}
	dx = x - g->mouse.last_x;
	sensitivity = 0.002;
	rot = dx * sensitivity;
	if (iabs_local(dx) > 1)
	{
		old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
	g->mouse.last_x = x;
	g->mouse.last_y = y;
	g->mouse.x = x;
	g->mouse.y = y;
	return (0);
}

void	apply_movement_bonus(t_game *g)
{
	double	nx;
	double	ny;
	double	buf;

	buf = 0.1;
	if (g->keys.w)
	{
		nx = g->pos_x + g->dir_x * g->move_speed;
		ny = g->pos_y + g->dir_y * g->move_speed;
		if (!check_collision(g, nx + (g->dir_x > 0 ? buf : -buf), g->pos_y))
			g->pos_x = nx;
		if (!check_collision(g, g->pos_x, ny + (g->dir_y > 0 ? buf : -buf)))
			g->pos_y = ny;
	}
	if (g->keys.s)
	{
		nx = g->pos_x - g->dir_x * g->move_speed;
		ny = g->pos_y - g->dir_y * g->move_speed;
		if (!check_collision(g, nx + (g->dir_x < 0 ? buf : -buf), g->pos_y))
			g->pos_x = nx;
		if (!check_collision(g, g->pos_x, ny + (g->dir_y < 0 ? buf : -buf)))
			g->pos_y = ny;
	}
	if (g->keys.a)
	{
		nx = g->pos_x - g->plane_x * g->move_speed;
		ny = g->pos_y - g->plane_y * g->move_speed;
		if (!check_collision(g, nx + (g->plane_x < 0 ? buf : -buf), g->pos_y))
			g->pos_x = nx;
		if (!check_collision(g, g->pos_x, ny + (g->plane_y < 0 ? buf : -buf)))
			g->pos_y = ny;
	}
	if (g->keys.d)
	{
		nx = g->pos_x + g->plane_x * g->move_speed;
		ny = g->pos_y + g->plane_y * g->move_speed;
		if (!check_collision(g, nx + (g->plane_x > 0 ? buf : -buf), g->pos_y))
			g->pos_x = nx;
		if (!check_collision(g, g->pos_x, ny + (g->plane_y > 0 ? buf : -buf)))
			g->pos_y = ny;
	}
	if (g->keys.left || g->keys.right)
	{
		double	rot_sign;
		double	rot;
		double	old_dir_x;
		double	old_plane_x;

		rot_sign = g->keys.left ? 1.0 : -1.0;
		rot = g->rot_speed * rot_sign;
		old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
}

