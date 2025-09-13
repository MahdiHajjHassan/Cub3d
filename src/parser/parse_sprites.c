/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sprites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 13:25:13 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

static int	copy_existing_sprites(
	t_config *cfg, t_sprite *new_sprites, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_sprites[i] = cfg->map.sprites[i];
		i++;
	}
	return (0);
}

static void	init_new_sprite(t_sprite *sprite, int x, int y)
{
	sprite->x = x + 0.5;
	sprite->y = y + 0.5;
	sprite->texture_id = 0;
	sprite->frame_count = 2;
	sprite->current_frame = 0;
	sprite->frame_timer = 0.0;
	sprite->animated = true;
	sprite->distance = 0.0;
}

int	add_sprite(t_config *cfg, int x, int y, char type)
{
	t_sprite	*new_sprites;

	(void)type;
	new_sprites = malloc(sizeof(t_sprite) * (cfg->map.sprite_count + 1));
	if (!new_sprites)
		return (error_msg("parse: memory error"));
	copy_existing_sprites(cfg, new_sprites, cfg->map.sprite_count);
	init_new_sprite(&new_sprites[cfg->map.sprite_count], x, y);
	if (cfg->map.sprites)
		free(cfg->map.sprites);
	cfg->map.sprites = new_sprites;
	cfg->map.sprite_count++;
	return (0);
}

int	add_door(t_config *cfg, int x, int y)
{
	t_door	*new_doors;
	int		i;

	new_doors = malloc(sizeof(t_door) * (cfg->map.door_count + 1));
	if (!new_doors)
		return (error_msg("parse: memory error"));
	i = 0;
	while (i < cfg->map.door_count)
	{
		new_doors[i] = cfg->map.doors[i];
		i++;
	}
	new_doors[cfg->map.door_count].x = x;
	new_doors[cfg->map.door_count].y = y;
	new_doors[cfg->map.door_count].is_open = false;
	new_doors[cfg->map.door_count].open_timer = 0.0;
	if (cfg->map.doors)
		free(cfg->map.doors);
	cfg->map.doors = new_doors;
	cfg->map.door_count++;
	return (0);
}
