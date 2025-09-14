/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:35:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:03:36 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <math.h>
#include <sys/time.h>

void	init_game_from_cfg(t_game *g, const t_config *cfg)
{
	g->map = &cfg->map;
	g->pos_x = (double)cfg->map.player_x + 0.5;
	g->pos_y = (double)cfg->map.player_y + 0.5;
	g->dir_x = cos(cfg->map.player_angle);
	g->dir_y = -sin(cfg->map.player_angle);
	g->plane_x = -g->dir_y * 0.66;
	g->plane_y = g->dir_x * 0.66;
	init_game_controls(g);
	init_game_speeds(g);
	init_game_colors(g, cfg);
	g->closing = 0;
	gettimeofday(&g->last_time, NULL);
	g->frame_time = 0.0;
}

void	init_game_controls(t_game *g)
{
	g->keys = (t_keys){0, 0, 0, 0, 0, 0, 0};
	g->mouse = (t_mouse){0, 0, 0, 0, false};
}

void	init_game_speeds(t_game *g)
{
	g->move_speed = 3.0;
	g->rot_speed = 3.0;
}

void	init_game_colors(t_game *g, const t_config *cfg)
{
	g->ceil_color = cfg->ceiling_color.value;
	g->floor_color = cfg->floor_color.value;
}
