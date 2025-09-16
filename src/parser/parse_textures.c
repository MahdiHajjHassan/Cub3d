/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:53:20 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

static int	handle_no_texture(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "NO ") && !flags->no)
	{
		if (!validate_texture_file(trimmed + 3))
			return (error_msg("parse: NO texture file not found"));
		out_cfg->textures.no = ft_strdup(trimmed + 3);
		flags->no = 1;
		return (1);
	}
	return (0);
}

static int	handle_so_texture(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "SO ") && !flags->so)
	{
		if (!validate_texture_file(trimmed + 3))
			return (error_msg("parse: SO texture file not found"));
		out_cfg->textures.so = ft_strdup(trimmed + 3);
		flags->so = 1;
		return (1);
	}
	return (0);
}

static int	handle_ea_texture(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "EA ") && !flags->ea)
	{
		if (!validate_texture_file(trimmed + 3))
			return (error_msg("parse: EA texture file not found"));
		out_cfg->textures.ea = ft_strdup(trimmed + 3);
		flags->ea = 1;
		return (1);
	}
	return (0);
}

static int	handle_we_texture(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "WE ") && !flags->we)
	{
		if (!validate_texture_file(trimmed + 3))
			return (error_msg("parse: WE texture file not found"));
		out_cfg->textures.we = ft_strdup(trimmed + 3);
		flags->we = 1;
		return (1);
	}
	return (0);
}

int	handle_wall_textures(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (handle_no_texture(trimmed, out_cfg, flags))
		return (1);
	if (handle_so_texture(trimmed, out_cfg, flags))
		return (1);
	if (handle_ea_texture(trimmed, out_cfg, flags))
		return (1);
	if (handle_we_texture(trimmed, out_cfg, flags))
		return (1);
	return (0);
}
