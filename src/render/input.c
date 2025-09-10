/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:36 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/10 16:45:44 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

/* These helpers live in separate files to satisfy Norm limits */
void	handle_door_interaction(t_game *g);
void	update_doors(t_game *g);
int		on_mouse_move(int x, int y, void *param);

int	on_key_press(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (keycode == KEY_ESC)
	{
		g->closing = 1;
		if (g->mlx && g->win)
			mlx_mouse_show(g->mlx, g->win);
		mlx_loop_end(g->mlx);
	}
	else if (keycode == KEY_W)
		g->keys.w = 1;
	else if (keycode == KEY_A)
		g->keys.a = 1;
	else if (keycode == KEY_S)
		g->keys.s = 1;
	else if (keycode == KEY_D)
		g->keys.d = 1;
	else if (keycode == KEY_LEFT)
		g->keys.left = 1;
	else if (keycode == KEY_RIGHT)
		g->keys.right = 1;
	else if (keycode == KEY_SPACE)
	{
		g->keys.space = 1;
		handle_door_interaction(g);
	}
	return (0);
}

int	on_key_release(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (keycode == KEY_W)
		g->keys.w = 0;
	else if (keycode == KEY_A)
		g->keys.a = 0;
	else if (keycode == KEY_S)
		g->keys.s = 0;
	else if (keycode == KEY_D)
		g->keys.d = 0;
	else if (keycode == KEY_LEFT)
		g->keys.left = 0;
	else if (keycode == KEY_RIGHT)
		g->keys.right = 0;
	else if (keycode == KEY_SPACE)
		g->keys.space = 0;
	return (0);
}

int	on_destroy(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (g && !g->closing)
	{
		g->closing = 1;
		if (g->mlx && g->win)
			mlx_mouse_show(g->mlx, g->win);
		mlx_loop_end(g->mlx);
	}
	return (0);
}

int	game_loop(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (!g || g->closing)
		return (0);
	g->frame_time = get_time_delta(&g->last_time);
	apply_movement(g);
	update_doors(g);
	update_sprites(g);
	render_frame(g);
	return (0);
}
