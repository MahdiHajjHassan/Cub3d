/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_update.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:05:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:57:12 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

static void	update_sprite_distance(t_game *g, t_sprite *sprite)
{
	double	dx;
	double	dy;

	dx = sprite->x - g->pos_x;
	dy = sprite->y - g->pos_y;
	sprite->distance = dx * dx + dy * dy;
}

static void	update_sprite_animation(t_game *g, t_sprite *sprite)
{
	double	frame_duration;

	frame_duration = 0.5;
	if (sprite->animated && sprite->frame_count > 1)
	{
		sprite->frame_timer += g->frame_time;
		if (sprite->frame_timer >= frame_duration)
		{
			sprite->current_frame = (sprite->current_frame + 1)
				% sprite->frame_count;
			sprite->frame_timer = 0.0;
		}
	}
}

void	update_sprites(t_game *g)
{
	int			i;
	t_sprite	*sprite;

	i = 0;
	while (i < g->map->sprite_count)
	{
		sprite = &g->map->sprites[i];
		update_sprite_distance(g, sprite);
		update_sprite_animation(g, sprite);
		i++;
	}
	sort_sprites_by_distance(g);
}
