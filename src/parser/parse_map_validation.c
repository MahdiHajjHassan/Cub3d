/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:00:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	validate_map_characters(char **map_lines, size_t height)
{
	size_t	i;
	size_t	j;
	int		player_count;

	player_count = 0;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (map_lines[i][j])
		{
			if (!ft_strchr("01NSEWFD2 \t", map_lines[i][j]))
				return (error_msg("parse: invalid map character"));
			if (ft_strchr("NSEW", map_lines[i][j]))
				player_count++;
			j++;
		}
		i++;
	}
	if (player_count != 1)
		return (error_msg("parse: map must have exactly one player"));
	return (0);
}

int	is_valid_map_position(char **map_lines, size_t height, size_t row,
		size_t col)
{
	if (row >= height || col >= ft_strlen(map_lines[row]))
		return (0);
	return (ft_strchr("01NSEWFD2", map_lines[row][col]) != NULL);
}

int	validate_and_parse_map(t_parse_context *ctx, size_t map_start)
{
	if (validate_required_elements(ctx->flags) != 0)
		return (1);
	if (map_start == 0)
		return (error_msg("parse: no map found"));
	if (parse_map(ctx->lines, ctx->count, map_start, ctx->cfg) != 0)
		return (1);
	return (0);
}
