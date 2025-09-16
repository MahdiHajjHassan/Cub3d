/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 14:35:55 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	allocate_map_row(t_config *cfg, char **lines, size_t line_idx,
	size_t row_idx)
{
	size_t	j;

	cfg->map.grid[row_idx] = malloc(cfg->map.width + 1);
	if (!cfg->map.grid[row_idx])
	{
		cfg->map.height = row_idx;
		return (error_msg("parse: memory error"));
	}
	ft_strlcpy(cfg->map.grid[row_idx], lines[line_idx], cfg->map.width + 1);
	j = ft_strlen(cfg->map.grid[row_idx]);
	while (j < cfg->map.width)
	{
		cfg->map.grid[row_idx][j] = ' ';
		j++;
	}
	cfg->map.grid[row_idx][cfg->map.width] = '\0';
	return (0);
}
