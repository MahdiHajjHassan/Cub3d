/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 13:25:17 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	handle_map_char(t_config *cfg, t_char_position *pos)
{
	if (pos->c == 'N' || pos->c == 'S' || pos->c == 'E' || pos->c == 'W')
		return (handle_player_char(cfg, pos));
	else if (pos->c == 'D')
		return (handle_door_char(cfg, pos->i, pos->j));
	else if (pos->c == '2')
		return (handle_sprite_char(cfg, pos->c, pos->i, pos->j));
	else if (pos->c != '0' && pos->c != '1' && pos->c != ' ')
		return (error_msg("parse: invalid map character"));
	return (0);
}

int	parse_map_cells(t_config *cfg, int *player_count)
{
	size_t				i;
	size_t				j;
	char				c;
	t_char_position		pos;

	i = 0;
	while (i < cfg->map.height)
	{
		j = 0;
		while (j < cfg->map.width)
		{
			c = cfg->map.grid[i][j];
			pos.c = c;
			pos.i = i;
			pos.j = j;
			pos.player_count = player_count;
			if (handle_map_char(cfg, &pos) != 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	parse_map(char **lines, size_t count, size_t map_start, t_config *cfg)
{
	int	player_count;

	cfg->map.height = count - map_start;
	cfg->map.grid = malloc(sizeof(char *) * cfg->map.height);
	if (!cfg->map.grid)
		return (error_msg("parse: memory error"));
	cfg->map.width = get_max_width(lines, map_start, count);
	if (fill_map_grid(cfg, lines, map_start, count) != 0)
		return (1);
	player_count = 0;
	if (parse_map_cells(cfg, &player_count) != 0)
		return (1);
	if (player_count == 0)
		return (error_msg("parse: no player found"));
	return (validate_map(&cfg->map));
}

int	validate_map_borders(char **map_lines, size_t height)
{
	size_t	i;
	size_t	j;
	size_t	width;

	i = 0;
	while (i < height)
	{
		width = ft_strlen(map_lines[i]);
		j = 0;
		while (j < width)
		{
			if (ft_strchr("0NSEWFD2", map_lines[i][j]))
			{
				if (!is_valid_map_position(map_lines, height, i - 1, j)
					|| !is_valid_map_position(map_lines, height, i + 1, j)
					|| !is_valid_map_position(map_lines, height, i, j - 1)
					|| !is_valid_map_position(map_lines, height, i, j + 1))
					return (error_msg("parse: map not closed"));
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_all_map(char **map_lines, size_t height)
{
	if (validate_map_characters(map_lines, height) != 0)
		return (1);
	if (validate_map_borders(map_lines, height) != 0)
		return (1);
	return (0);
}
