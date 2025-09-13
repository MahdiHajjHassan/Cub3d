/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:50:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:45:43 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	setup_game_hooks(t_game *g)
{
	mlx_hook(g->win, 2, 1L << 0, &on_key_press, g);
	mlx_hook(g->win, 3, 1L << 1, &on_key_release, g);
	mlx_hook(g->win, 17, 0, &on_destroy, g);
	mlx_hook(g->win, 6, 1L << 6, &on_mouse_move, g);
	mlx_loop_hook(g->mlx, &game_loop, g);
}

static void	setup_mouse_capture(t_game *g)
{
	if (g->mlx && g->win)
	{
		mlx_mouse_hide(g->mlx, g->win);
		mlx_mouse_move(g->mlx, g->win, WIN_W / 2, WIN_H / 2);
	}
	g->mouse.captured = true;
	g->mouse.x = WIN_W / 2;
	g->mouse.last_x = WIN_W / 2;
	g->mouse.y = WIN_H / 2;
	g->mouse.last_y = WIN_H / 2;
}

int	run_game(const t_config *cfg)
{
	t_game	g;

	if (!display_available())
		return (error_msg("no DISPLAY; run under X11 or use xvfb-run"));
	init_game_structures(&g);
	if (setup_mlx_window(&g) != 0)
		return (1);
	if (setup_game_resources(&g, cfg) != 0)
		return (1);
	setup_game_hooks(&g);
	setup_mouse_capture(&g);
	mlx_loop(g.mlx);
	cleanup_game_complete(&g);
	return (0);
}
