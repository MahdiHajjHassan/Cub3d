/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_mouse_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:05:15 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 20:05:15 by hsharaf-         ###   ########.fr       */
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

int	on_mouse_move(int x, int y, void *param)
{
	t_game	*g;
	int		dx;
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
		g->mouse.captured = 1;
		return (0);
	}
	if (!g->mouse.captured)
	{
		g->mouse.x = x;
		g->mouse.y = y;
		g->mouse.last_x = x;
		g->mouse.last_y = y;
		g->mouse.captured = 1;
		return (0);
	}
	dx = x - g->mouse.last_x;
	rot = (double)dx * 0.002;
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
