/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:53:20 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

static int	validate_and_split(char *trimmed, char **comma1, char **comma2)
{
	*comma1 = ft_strchr(trimmed, ',');
	if (!*comma1)
	{
		free(trimmed);
		return (error_msg("parse: color format error"));
	}
	**comma1 = '\0';
	*comma2 = ft_strchr(*comma1 + 1, ',');
	if (!*comma2)
	{
		free(trimmed);
		return (error_msg("parse: color format error"));
	}
	**comma2 = '\0';
	return (0);
}

static int	assign_rgb_values(
	char *trimmed, char *comma1, char *comma2, t_color *out_color)
{
	int	r;
	int	g;
	int	b;

	r = ft_atoi(trimmed);
	g = ft_atoi(comma1 + 1);
	b = ft_atoi(comma2 + 1);
	free(trimmed);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (error_msg("parse: RGB values must be 0-255"));
	out_color->r = r;
	out_color->g = g;
	out_color->b = b;
	out_color->value = (r << 16) | (g << 8) | b;
	return (0);
}

int	parse_color(const char *line, t_color *out_color)
{
	char	*trimmed;
	char	*comma1;
	char	*comma2;

	trimmed = str_trim_spaces(line);
	if (!trimmed)
		return (error_msg("parse: memory error"));
	if (validate_and_split(trimmed, &comma1, &comma2) != 0)
		return (-1);
	return (assign_rgb_values(trimmed, comma1, comma2, out_color));
}

t_parse_flags	init_flags(void)
{
	t_parse_flags	flags;

	flags.no = 0;
	flags.so = 0;
	flags.ea = 0;
	flags.we = 0;
	flags.f = 0;
	flags.c = 0;
	return (flags);
}

int	handle_color_header(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "F ") && !flags->f)
	{
		if (parse_color(trimmed + 2, &out_cfg->floor_color) != 0)
			return (-1);
		flags->f = 1;
		return (1);
	}
	else if (starts_with(trimmed, "C ") && !flags->c)
	{
		if (parse_color(trimmed + 2, &out_cfg->ceiling_color) != 0)
			return (-1);
		flags->c = 1;
		return (1);
	}
	return (0);
}
