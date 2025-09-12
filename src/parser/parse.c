/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:49:09 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/10 16:49:13 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_color(const char *line, t_color *out_color);
static int	parse_map(char **lines, size_t count, size_t map_start,
		t_config *cfg);
static int	validate_map(const t_map *map);
static void	init_config(t_config *cfg);
static int	parse_header_line(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags);
static int	validate_required_elements(const t_parse_flags *flags);
static int	parse_headers(char **lines, size_t count, t_config *out_cfg,
		t_parse_flags *flags, size_t *map_start);
static int	validate_and_parse_map(char **lines, size_t count,
		t_config *out_cfg, t_parse_flags *flags, size_t map_start);

static void	free_textures(t_config *cfg)
{
	if (cfg->textures.no)
		free(cfg->textures.no);
	if (cfg->textures.so)
		free(cfg->textures.so);
	if (cfg->textures.ea)
		free(cfg->textures.ea);
	if (cfg->textures.we)
		free(cfg->textures.we);
	if (cfg->textures.door)
		free(cfg->textures.door);
}

static void	free_sprites(t_config *cfg)
{
	int	i;

	if (!cfg->textures.sprites)
		return ;
	i = 0;
	while (i < cfg->textures.sprite_count)
	{
		if (cfg->textures.sprites[i])
			free(cfg->textures.sprites[i]);
		i++;
	}
	free(cfg->textures.sprites);
}

static void	free_map(t_config *cfg)
{
	free_lines(cfg->map.grid, cfg->map.height);
	if (cfg->map.doors)
		free(cfg->map.doors);
	if (cfg->map.sprites)
		free(cfg->map.sprites);
}

void	free_config(t_config *cfg)
{
	free_textures(cfg);
	free_sprites(cfg);
	free_map(cfg);
}

static void	init_config(t_config *cfg)
{
	cfg->textures.no = NULL;
	cfg->textures.so = NULL;
	cfg->textures.ea = NULL;
	cfg->textures.we = NULL;
	cfg->textures.door = NULL;
	cfg->textures.sprites = NULL;
	cfg->textures.sprite_count = 0;
	cfg->floor_color = (t_color){-1, -1, -1, -1};
	cfg->ceiling_color = (t_color){-1, -1, -1, -1};
	cfg->map.grid = NULL;
	cfg->map.width = 0;
	cfg->map.height = 0;
	cfg->map.doors = NULL;
	cfg->map.door_count = 0;
	cfg->map.sprites = NULL;
	cfg->map.sprite_count = 0;
}

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

static int	parse_color(const char *line, t_color *out_color)
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

static int	copy_existing_sprites(
	t_config *cfg, t_sprite *new_sprites, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_sprites[i] = cfg->map.sprites[i];
		i++;
	}
	return (0);
}

static void	init_new_sprite(t_sprite *sprite, int x, int y)
{
	sprite->x = x + 0.5;
	sprite->y = y + 0.5;
	sprite->texture_id = 0;
	sprite->frame_count = 2;
	sprite->current_frame = 0;
	sprite->frame_timer = 0.0;
	sprite->animated = true;
	sprite->distance = 0.0;
}

static int	add_sprite(t_config *cfg, int x, int y, char type)
{
	t_sprite	*new_sprites;

	(void)type;
	new_sprites = malloc(sizeof(t_sprite) * (cfg->map.sprite_count + 1));
	if (!new_sprites)
		return (error_msg("parse: memory error"));
	copy_existing_sprites(cfg, new_sprites, cfg->map.sprite_count);
	init_new_sprite(&new_sprites[cfg->map.sprite_count], x, y);
	if (cfg->map.sprites)
		free(cfg->map.sprites);
	cfg->map.sprites = new_sprites;
	cfg->map.sprite_count++;
	return (0);
}

static int	add_door(t_config *cfg, int x, int y)
{
	t_door	*new_doors;
	int		i;

	new_doors = malloc(sizeof(t_door) * (cfg->map.door_count + 1));
	if (!new_doors)
		return (error_msg("parse: memory error"));
	i = 0;
	while (i < cfg->map.door_count)
	{
		new_doors[i] = cfg->map.doors[i];
		i++;
	}
	new_doors[cfg->map.door_count].x = x;
	new_doors[cfg->map.door_count].y = y;
	new_doors[cfg->map.door_count].is_open = false;
	new_doors[cfg->map.door_count].open_timer = 0.0;
	if (cfg->map.doors)
		free(cfg->map.doors);
	cfg->map.doors = new_doors;
	cfg->map.door_count++;
	return (0);
}


static size_t	get_max_width(char **lines, size_t start, size_t count)
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

static int	fill_map_grid(
	t_config *cfg, char **lines, size_t start, size_t count)
	{
	size_t	i;
	size_t	j;

	i = start;
	while (i < count)
	{
		cfg->map.grid[i - start] = malloc(cfg->map.width + 1);
		if (!cfg->map.grid[i - start])
		return (error_msg("parse: memory error"));
		ft_strlcpy(cfg->map.grid[i - start], lines[i], cfg->map.width + 1);
		j = ft_strlen(cfg->map.grid[i - start]);
		while (j < cfg->map.width)
		{
			cfg->map.grid[i - start][j] = ' ';
			j++;
		}
		cfg->map.grid[i - start][cfg->map.width] = '\0';
		i++;
	}
	return (0);
}

static int	handle_map_char(
	t_config *cfg, char c, size_t i, size_t j, int *player_count)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		if (*player_count > 0)
			return (error_msg("parse: multiple players found"));
		cfg->map.player_x = j;
		cfg->map.player_y = i;
		cfg->map.player_angle = (c == 'N') ? M_PI / 2 :
			(c == 'S') ? -M_PI / 2 : (c == 'E') ? 0 : M_PI;
		cfg->map.grid[i][j] = '0';
		(*player_count)++;
	}
	else if (c == 'D')
	{
		if (add_door(cfg, j, i) != 0)
			return (1);
		cfg->map.grid[i][j] = 'D';
	}
	else if (c == '2')
	{
		if (add_sprite(cfg, j, i, c) != 0)
			return (1);
		cfg->map.grid[i][j] = '0';
	}
	else if (c != '0' && c != '1' && c != ' ')
		return (error_msg("parse: invalid map character"));
	return (0);
}

static int	parse_map_cells(t_config *cfg, int *player_count)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (i < cfg->map.height)
	{
		j = 0;
		while (j < cfg->map.width)
		{
			c = cfg->map.grid[i][j];
			if (handle_map_char(cfg, c, i, j, player_count) != 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

static int	parse_map(
	char **lines, size_t count, size_t map_start, t_config *cfg)
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

static int	validate_map(const t_map *map)
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

t_parse_flags	init_flags(void)
{
	t_parse_flags flags;

	flags.no = 0;
	flags.so = 0;
	flags.ea = 0;
	flags.we = 0;
	flags.f = 0;
	flags.c = 0;
	return flags;
}

static int	parse_header_line(const char *trimmed, t_config *out_cfg,
		t_parse_flags *flags)
{
	if (starts_with(trimmed, "NO ") && !flags->no)
	{
		out_cfg->textures.no = ft_strdup(trimmed + 3);
		flags->no = 1;
	}
	else if (starts_with(trimmed, "SO ") && !flags->so)
	{
		out_cfg->textures.so = ft_strdup(trimmed + 3);
		flags->so = 1;
	}
	else if (starts_with(trimmed, "EA ") && !flags->ea)
	{
		out_cfg->textures.ea = ft_strdup(trimmed + 3);
		flags->ea = 1;
	}
	else if (starts_with(trimmed, "WE ") && !flags->we)
	{
		out_cfg->textures.we = ft_strdup(trimmed + 3);
		flags->we = 1;
	}
	else if (starts_with(trimmed, "DO "))
	{
		out_cfg->textures.door = ft_strdup(trimmed + 3);
	}
	else if (starts_with(trimmed, "F ") && !flags->f)
	{
		if (parse_color(trimmed + 2, &out_cfg->floor_color) != 0)
			return (1);
		flags->f = 1;
	}
	else if (starts_with(trimmed, "C ") && !flags->c)
	{
		if (parse_color(trimmed + 2, &out_cfg->ceiling_color) != 0)
			return (1);
		flags->c = 1;
	}
	else if (ft_strchr(trimmed, '1') || ft_strchr(trimmed, '0')
		|| ft_strchr(trimmed, 'N') || ft_strchr(trimmed, 'S')
		|| ft_strchr(trimmed, 'E') || ft_strchr(trimmed, 'W')
		|| ft_strchr(trimmed, 'D') || ft_strchr(trimmed, '2'))
	{
		return (2);
	}
	return (0);
}

static int	validate_required_elements(const t_parse_flags *flags)
{
	if (!flags->no || !flags->so || !flags->ea || !flags->we
		|| !flags->f || !flags->c)
		return (error_msg("parse: missing required elements"));
	return (0);
}

static int	parse_headers(char **lines, size_t count, t_config *out_cfg,
		t_parse_flags *flags, size_t *map_start)
{
	char	*trimmed;
	int		result;

	while (*map_start < count)
	{
		trimmed = str_trim_spaces(lines[*map_start]);
		if (!trimmed)
		{
			free_lines(lines, count);
			return (error_msg("parse: memory error"));
		}
		if (ft_strlen(trimmed) == 0 || trimmed[0] == '\0')
		{
			free(trimmed);
			(*map_start)++;
			continue ;
		}
		result = parse_header_line(trimmed, out_cfg, flags);
		if (result == 1)
		{
			free(trimmed);
			free_lines(lines, count);
			return (1);
		}
		if (result == 2)
		{
			free(trimmed);
			break ;
		}
		free(trimmed);
		(*map_start)++;
	}
	return (0);
}

static int	validate_and_parse_map(char **lines, size_t count,
		t_config *out_cfg, t_parse_flags *flags, size_t map_start)
{
	if (validate_required_elements(flags) != 0)
	{
		free_lines(lines, count);
		return (1);
	}
	if (map_start == 0)
	{
		free_lines(lines, count);
		return (error_msg("parse: no map found"));
	}
	if (parse_map(lines, count, map_start, out_cfg) != 0)
	{
		free_lines(lines, count);
		return (1);
	}
	return (0);
}
int	parse_cub_file(const char *path, t_config *out_cfg)
{
	char			**lines;
	size_t			count;
	t_parse_flags	flags;
	size_t			map_start;

	map_start = 0;
	flags = init_flags();
	init_config(out_cfg);
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	if (parse_headers(lines, count, out_cfg, &flags, &map_start) != 0)
		return (1);
	if (validate_and_parse_map(lines, count, out_cfg, &flags, map_start) != 0)
		return (1);
	free_lines(lines, count);
	return (0);
}
