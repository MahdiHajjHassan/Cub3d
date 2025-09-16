/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 14:35:55 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

size_t	get_max_width(char **lines, size_t start, size_t count)
{
	size_t	i;
	size_t	max_width;

	i = start;
	max_width = 0;
	while (i < count)
	{
		if (ft_strlen(lines[i]) > max_width)
			max_width = ft_strlen(lines[i]);
		i++;
	}
	return (max_width);
}

int	fill_map_grid(t_config *cfg, char **lines, size_t start, size_t count)
{
	size_t	i;
	size_t	allocated_rows;

	i = start;
	allocated_rows = 0;
	while (i < count)
	{
		if (allocate_map_row(cfg, lines, i, allocated_rows) != 0)
			return (1);
		i++;
		allocated_rows++;
	}
	return (0);
}

int	handle_player_char(t_config *cfg, t_char_position *pos)
{
	if (*(pos->player_count) > 0)
		return (error_msg("parse: multiple players found"));
	cfg->map.player_x = pos->j;
	cfg->map.player_y = pos->i;
	if (pos->c == 'N')
		cfg->map.player_angle = M_PI / 2;
	else if (pos->c == 'S')
		cfg->map.player_angle = -M_PI / 2;
	else if (pos->c == 'E')
		cfg->map.player_angle = 0;
	else
		cfg->map.player_angle = M_PI;
	cfg->map.grid[pos->i][pos->j] = '0';
	(*(pos->player_count))++;
	return (0);
}

int	handle_door_char(t_config *cfg, size_t i, size_t j)
{
	if (add_door(cfg, j, i) != 0)
		return (1);
	cfg->map.grid[i][j] = 'D';
	return (0);
}

int	handle_sprite_char(t_config *cfg, char c, size_t i, size_t j)
{
	if (add_sprite(cfg, j, i, c) != 0)
		return (1);
	cfg->map.grid[i][j] = '0';
	return (0);
}
