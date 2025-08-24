/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:35:10 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 19:12:30 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

int		apply_directive(char *line, t_config *cfg, int *header_done);
int		is_map_line(const char *line);
int		count_map_lines(char **lines, size_t start, size_t total, size_t *out);
size_t	max_line_width(char **lines, size_t start, size_t total);
int		build_grid(t_map *map, char **lines, size_t start);
int		detect_spawn(t_map *map);
int		validate_closed(const t_map *map);
int		find_map_start_and_apply(char **lines, size_t count,
								t_config *cfg, size_t *idx);

static void	init_config(t_config *cfg)
{
	cfg->textures.no = NULL;
	cfg->textures.so = NULL;
	cfg->textures.ea = NULL;
	cfg->textures.we = NULL;
	cfg->floor_color.r = -1;
	cfg->floor_color.g = -1;
	cfg->floor_color.b = -1;
	cfg->floor_color.value = -1;
	cfg->ceiling_color.r = -1;
	cfg->ceiling_color.g = -1;
	cfg->ceiling_color.b = -1;
	cfg->ceiling_color.value = -1;
	cfg->map.grid = NULL;
	cfg->map.width = 0;
	cfg->map.height = 0;
	cfg->map.player_x = 0;
	cfg->map.player_y = 0;
	cfg->map.player_angle = 0.0;
}

void	free_config(t_config *cfg)
{
	size_t	i;

	if (!cfg)
		return ;
	free(cfg->textures.no);
	free(cfg->textures.so);
	free(cfg->textures.ea);
	free(cfg->textures.we);
	i = 0;
	if (cfg->map.grid)
	{
		while (i < cfg->map.height)
		{
			free(cfg->map.grid[i]);
			i++;
		}
		free(cfg->map.grid);
	}
}

static int	validate_header(const t_config *cfg)
{
	if (!cfg->textures.no || !cfg->textures.so)
		return (error_msg("missing NO/SO"));
	if (!cfg->textures.ea || !cfg->textures.we)
		return (error_msg("missing EA/WE"));
	if (cfg->floor_color.value < 0)
		return (error_msg("missing floor color"));
	if (cfg->ceiling_color.value < 0)
		return (error_msg("missing ceiling color"));
	return (0);
}

int	parse_cub_file(const char *path, t_config *out_cfg)
{
	char	**lines;
	size_t	count;
	size_t	idx;

	if (!path || !out_cfg)
		return (error_msg("internal: invalid args"));
	init_config(out_cfg);
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	idx = 0;
	if (find_map_start_and_apply(lines, count, out_cfg, &idx) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	if (validate_header(out_cfg) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	if (idx >= count)
		return (free_lines(lines, count), free_config(out_cfg),
			error_msg("missing map"));
	if (count_map_lines(lines, idx, count, &out_cfg->map.height) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	out_cfg->map.width = max_line_width(lines, idx, count);
	if (out_cfg->map.width == 0 || out_cfg->map.height == 0)
		return (free_lines(lines, count), free_config(out_cfg),
			error_msg("empty map"));
	if (build_grid(&out_cfg->map, lines, idx) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	free_lines(lines, count);
	if (detect_spawn(&out_cfg->map) != 0)
		return (free_config(out_cfg), 1);
	if (validate_closed(&out_cfg->map) != 0)
		return (free_config(out_cfg), 1);
	return (0);
}
