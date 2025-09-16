/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:53:20 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	validate_texture_file(const char *path)
{
	int	fd;

	if (!path || !*path)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	close(fd);
	return (1);
}

int	handle_door_texture(const char *trimmed, t_config *out_cfg)
{
	if (starts_with(trimmed, "DO "))
	{
		if (!validate_texture_file(trimmed + 3))
			return (error_msg("parse: DO texture file not found"));
		out_cfg->textures.door = ft_strdup(trimmed + 3);
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

int	handle_texture_header(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (handle_wall_textures(trimmed, out_cfg, flags))
		return (1);
	if (handle_door_texture(trimmed, out_cfg))
		return (1);
	return (0);
}
