/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:40:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/15 00:30:27 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

void	init_game_structures(t_game *g)
{
	int	i;

	g->mlx = NULL;
	g->win = NULL;
	g->frame = (t_img){0};
	g->minimap = (t_minimap){0};
	i = 0;
	while (i < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		g->tex[i] = (t_img){0};
		i++;
	}
}

int	setup_mlx_window(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (error_msg("mlx: init failed"));
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "CYBERFORTRESS");
	if (!g->win)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
		return (error_msg("mlx: window failed"));
	}
	center_window_on_screen(g->mlx, g->win);
	return (0);
}

int	setup_game_resources(t_game *g, const t_config *cfg)
{
	if (frame_init(g) != 0)
		return (cleanup_mlx_window(g), 1);
	init_game_from_cfg(g, cfg);
	if (textures_load(g, cfg) != 0)
		return (cleanup_game_partial(g), 1);
	if (init_minimap(g) != 0)
		return (cleanup_game_full(g), 1);
	return (0);
}
