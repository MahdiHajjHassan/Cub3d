/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:36 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 17:55:37 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

void	handle_door_interaction(t_game *g);
void	update_doors(t_game *g);
int		on_mouse_move(int x, int y, void *param);

static void	set_key_state(t_game *g, int keycode, int state)
{
	if (keycode == KEY_W)
		g->keys.w = state;
	else if (keycode == KEY_A)
		g->keys.a = state;
	else if (keycode == KEY_S)
		g->keys.s = state;
	else if (keycode == KEY_D)
		g->keys.d = state;
	else if (keycode == KEY_LEFT)
		g->keys.left = state;
	else if (keycode == KEY_RIGHT)
		g->keys.right = state;
	else if (keycode == KEY_SPACE)
		g->keys.space = state;
}

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
	else
	{
		set_key_state(g, keycode, 1);
		if (keycode == KEY_SPACE)
			handle_door_interaction(g);
	}
	return (0);
}

int	on_key_release(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	set_key_state(g, keycode, 0);
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
