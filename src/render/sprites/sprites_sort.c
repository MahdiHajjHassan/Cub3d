/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:10:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:57:12 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"

static void	swap_sprites(t_sprite *a, t_sprite *b)
{
	t_sprite	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_sprites_by_distance(t_game *g)
{
	int	i;
	int	j;

	i = 0;
	while (i < g->map->sprite_count - 1)
	{
		j = 0;
		while (j < g->map->sprite_count - i - 1)
		{
			if (g->map->sprites[j].distance < g->map->sprites[j + 1].distance)
				swap_sprites(&g->map->sprites[j], &g->map->sprites[j + 1]);
			j++;
		}
		i++;
	}
}
