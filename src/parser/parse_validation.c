/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:00:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	validate_map(const t_map *map)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			c = map->grid[i][j];
			if (c == '0' || c == 'D')
			{
				if (i == 0 || i == map->height - 1 || j == 0
					|| j == map->width - 1)
					return (error_msg("parse: map not closed"));
				if (map->grid[i - 1][j] == ' ' || map->grid[i + 1][j] == ' ' ||
					map->grid[i][j - 1] == ' ' || map->grid[i][j + 1] == ' ')
					return (error_msg("parse: map not closed"));
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_required_elements(const t_parse_flags *flags)
{
	if (!flags->no || !flags->so || !flags->ea || !flags->we
		|| !flags->f || !flags->c)
		return (error_msg("parse: missing required elements"));
	return (0);
}

int	handle_empty_line(char *trimmed, size_t *map_start)
{
	if (ft_strlen(trimmed) == 0 || trimmed[0] == '\0')
	{
		free(trimmed);
		(*map_start)++;
		return (1);
	}
	return (0);
}

int	process_header_line(t_header_context *ctx)
{
	int	result;

	result = parse_header_line(ctx->trimmed, ctx->out_cfg, ctx->flags);
	if (result == 1)
	{
		free(ctx->trimmed);
		return (1);
	}
	if (result == 2)
	{
		free(ctx->trimmed);
		return (2);
	}
	return (0);
}

int	parse_header_line(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	int	result;

	result = handle_texture_header(trimmed, out_cfg, flags);
	if (result)
		return (0);
	result = handle_color_header(trimmed, out_cfg, flags);
	if (result == -1)
		return (1);
	if (result)
		return (0);
	if (is_map_start(trimmed))
		return (2);
	return (0);
}
