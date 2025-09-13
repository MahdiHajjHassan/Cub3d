/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:45:43 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

void	cleanup_mlx_window(t_game *g)
{
	if (g->mlx && g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
}

void	cleanup_game_partial(t_game *g)
{
	frame_destroy(g);
	cleanup_mlx_window(g);
}

void	cleanup_game_full(t_game *g)
{
	textures_destroy(g);
	cleanup_game_partial(g);
}

void	cleanup_game_complete(t_game *g)
{
	if (g->mlx && g->win)
		mlx_mouse_show(g->mlx, g->win);
	destroy_minimap(g);
	cleanup_game_full(g);
}
