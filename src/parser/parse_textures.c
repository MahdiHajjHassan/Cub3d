/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:30:00 by kali             #+#    #+#             */
/*   Updated: 2025/09/13 12:30:00 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	handle_wall_textures(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "NO ") && !flags->no)
	{
		out_cfg->textures.no = ft_strdup(trimmed + 3);
		flags->no = 1;
		return (1);
	}
	else if (starts_with(trimmed, "SO ") && !flags->so)
	{
		out_cfg->textures.so = ft_strdup(trimmed + 3);
		flags->so = 1;
		return (1);
	}
	else if (starts_with(trimmed, "EA ") && !flags->ea)
	{
		out_cfg->textures.ea = ft_strdup(trimmed + 3);
		flags->ea = 1;
		return (1);
	}
	else if (starts_with(trimmed, "WE ") && !flags->we)
	{
		out_cfg->textures.we = ft_strdup(trimmed + 3);
		flags->we = 1;
		return (1);
	}
	return (0);
}

int	handle_door_texture(const char *trimmed, t_config *out_cfg)
{
	if (starts_with(trimmed, "DO "))
	{
		out_cfg->textures.door = ft_strdup(trimmed + 3);
		return (1);
	}
	return (0);
}

int	handle_texture_header(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (handle_wall_textures(trimmed, out_cfg, flags))
		return (1);
	if (handle_door_texture(trimmed, out_cfg))
		return (1);
	return (0);
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

int	is_map_start(const char *trimmed)
{
	if (ft_strchr(trimmed, '1') || ft_strchr(trimmed, '0')
		|| ft_strchr(trimmed, 'N') || ft_strchr(trimmed, 'S')
		|| ft_strchr(trimmed, 'E') || ft_strchr(trimmed, 'W')
		|| ft_strchr(trimmed, 'D') || ft_strchr(trimmed, '2'))
		return (1);
	return (0);
}
