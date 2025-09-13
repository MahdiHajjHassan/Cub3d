/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 23:28:50 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

void	render_sprites(t_game *g)
{
	int	i;

	i = 0;
	while (i < g->map->sprite_count)
	{
		render_single_sprite(g, &g->map->sprites[i]);
		i++;
	}
}
