/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 13:14:35 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

static void	free_textures(t_config *cfg)
{
	if (cfg->textures.no)
		free(cfg->textures.no);
	if (cfg->textures.so)
		free(cfg->textures.so);
	if (cfg->textures.ea)
		free(cfg->textures.ea);
	if (cfg->textures.we)
		free(cfg->textures.we);
	if (cfg->textures.door)
		free(cfg->textures.door);
}

static void	free_sprites(t_config *cfg)
{
	int	i;

	if (!cfg->textures.sprites)
		return ;
	i = 0;
	while (i < cfg->textures.sprite_count)
	{
		if (cfg->textures.sprites[i])
			free(cfg->textures.sprites[i]);
		i++;
	}
	free(cfg->textures.sprites);
}

static void	free_map(t_config *cfg)
{
	free_lines(cfg->map.grid, cfg->map.height);
	if (cfg->map.doors)
		free(cfg->map.doors);
	if (cfg->map.sprites)
		free(cfg->map.sprites);
}

void	free_config(t_config *cfg)
{
	free_textures(cfg);
	free_sprites(cfg);
	free_map(cfg);
}

void	init_config(t_config *cfg)
{
	cfg->textures.no = NULL;
	cfg->textures.so = NULL;
	cfg->textures.ea = NULL;
	cfg->textures.we = NULL;
	cfg->textures.door = NULL;
	cfg->textures.sprites = NULL;
	cfg->textures.sprite_count = 0;
	cfg->floor_color = (t_color){-1, -1, -1, -1};
	cfg->ceiling_color = (t_color){-1, -1, -1, -1};
	cfg->map.grid = NULL;
	cfg->map.width = 0;
	cfg->map.height = 0;
	cfg->map.doors = NULL;
	cfg->map.door_count = 0;
	cfg->map.sprites = NULL;
	cfg->map.sprite_count = 0;
}
