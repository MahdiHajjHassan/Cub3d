/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:22:01 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 18:22:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d_bonus.h"

static int parse_color(const char *line, t_color *out_color);
static int parse_map_bonus(char **lines, size_t count, size_t map_start, t_config *cfg);
static int validate_map_bonus(const t_map *map);
static void init_config_bonus(t_config *cfg);

void free_config(t_config *cfg)
{
	int i;

if (cfg->textures.no) free(cfg->textures.no);
	if (cfg->textures.so) free(cfg->textures.so);
	if (cfg->textures.ea) free(cfg->textures.ea);
	if (cfg->textures.we) free(cfg->textures.we);
	if (cfg->textures.door) free(cfg->textures.door);
	if (cfg->textures.sprites)
	{
		i = 0;
		while (i < cfg->textures.sprite_count)
		{
			if (cfg->textures.sprites[i])
				free(cfg->textures.sprites[i]);
			i++;
		}
		free(cfg->textures.sprites);
	}
	free_lines(cfg->map.grid, cfg->map.height);
	if (cfg->map.doors)
		free(cfg->map.doors);
	if (cfg->map.sprites)
		free(cfg->map.sprites);
}

static void init_config_bonus(t_config *cfg)
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

static int parse_color(const char *line, t_color *out_color)
{
	char *trimmed = str_trim_spaces(line);
	int r, g, b;
	char *comma1, *comma2;
	
	if (!trimmed)
		return (error_msg("parse: memory error"));
	comma1 = ft_strchr(trimmed, ',');
	if (!comma1)
	{
		free(trimmed);
		return (error_msg("parse: color format error"));
	}
	*comma1 = '\0';
	comma2 = ft_strchr(comma1 + 1, ',');
	if (!comma2)
	{
		free(trimmed);
		return (error_msg("parse: color format error"));
	}
	*comma2 = '\0';
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

static int add_sprite(t_config *cfg, int x, int y, char type)
{
	(void)type;
	t_sprite *new_sprites;
	int i;

	new_sprites = malloc(sizeof(t_sprite) * (cfg->map.sprite_count + 1));
	if (!new_sprites)
		return (error_msg("parse: memory error"));
	i = 0;
	while (i < cfg->map.sprite_count)
	{
		new_sprites[i] = cfg->map.sprites[i];
		i++;
	}
	new_sprites[cfg->map.sprite_count].x = x + 0.5;
	new_sprites[cfg->map.sprite_count].y = y + 0.5;
	new_sprites[cfg->map.sprite_count].texture_id = 0; /* Default sprite */
	new_sprites[cfg->map.sprite_count].frame_count = 4;
	new_sprites[cfg->map.sprite_count].current_frame = 0;
	new_sprites[cfg->map.sprite_count].frame_timer = 0.0;
	new_sprites[cfg->map.sprite_count].animated = true;
	new_sprites[cfg->map.sprite_count].distance = 0.0;
	if (cfg->map.sprites)
		free(cfg->map.sprites);
	cfg->map.sprites = new_sprites;
	cfg->map.sprite_count++;
	return (0);
}

static int add_door(t_config *cfg, int x, int y)
{
	t_door *new_doors;
	int i;

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

static int parse_map_bonus(char **lines, size_t count, size_t map_start, t_config *cfg)
{
	size_t i, j, max_width = 0;
	int player_count = 0;

	cfg->map.height = count - map_start;
	cfg->map.grid = malloc(sizeof(char *) * cfg->map.height);
	if (!cfg->map.grid)
		return (error_msg("parse: memory error"));

	/* Find maximum width */
	i = map_start;
	while (i < count)
	{
		if (ft_strlen(lines[i]) > max_width)
			max_width = ft_strlen(lines[i]);
		i++;
	}
	cfg->map.width = max_width;

	/* Copy and pad lines */
	i = map_start;
	while (i < count)
	{
		cfg->map.grid[i - map_start] = malloc(max_width + 1);
		if (!cfg->map.grid[i - map_start])
			return (error_msg("parse: memory error"));
		ft_strlcpy(cfg->map.grid[i - map_start], lines[i], max_width + 1);
		
		/* Pad with spaces if needed */
		j = ft_strlen(cfg->map.grid[i - map_start]);
		while (j < max_width)
		{
			cfg->map.grid[i - map_start][j] = ' ';
			j++;
		}
		cfg->map.grid[i - map_start][max_width] = '\0';
		i++;
	}

	/* Process map elements and validate */
	i = 0;
	while (i < cfg->map.height)
	{
		j = 0;
		while (j < cfg->map.width)
		{
			char c = cfg->map.grid[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (player_count > 0)
					return (error_msg("parse: multiple players found"));
				cfg->map.player_x = j;
				cfg->map.player_y = i;
				cfg->map.player_angle = (c == 'N') ? M_PI / 2 : (c == 'S') ? -M_PI / 2 : (c == 'E') ? 0 : M_PI;
				cfg->map.grid[i][j] = '0'; /* Replace with empty space */
				player_count++;
			}
			else if (c == 'D')
			{
				if (add_door(cfg, j, i) != 0)
					return (1);
				cfg->map.grid[i][j] = 'D'; /* Keep as door */
			}
			else if (c == '2')
			{
				if (add_sprite(cfg, j, i, c) != 0)
					return (1);
				cfg->map.grid[i][j] = '0'; /* Replace with empty space */
			}
			else if (c != '0' && c != '1' && c != ' ')
				return (error_msg("parse: invalid map character"));
			j++;
		}
		i++;
	}

	if (player_count == 0)
		return (error_msg("parse: no player found"));
	
	return (validate_map_bonus(&cfg->map));
}

static int validate_map_bonus(const t_map *map)
{
	size_t i, j;

	/* Check if map is closed */
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			char c = map->grid[i][j];
			if (c == '0' || c == 'D') /* Empty space or door needs walls around */
			{
				/* Check boundaries */
				if (i == 0 || i == map->height - 1 || j == 0 || j == map->width - 1)
					return (error_msg("parse: map not closed"));
				
				/* Check adjacent cells */
				if (map->grid[i-1][j] == ' ' || map->grid[i+1][j] == ' ' ||
					map->grid[i][j-1] == ' ' || map->grid[i][j+1] == ' ')
					return (error_msg("parse: map not closed"));
			}
			j++;
		}
		i++;
	}
	return (0);
}

int parse_cub_file(const char *path, t_config *out_cfg)
{
	char **lines;
	size_t count, i, map_start = 0;
	int no = 0, so = 0, ea = 0, we = 0, f = 0, c = 0;

	init_config_bonus(out_cfg);
	
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	
	i = 0;
	while (i < count)
	{
		char *trimmed = str_trim_spaces(lines[i]);
		if (!trimmed)
		{
			free_lines(lines, count);
			return (error_msg("parse: memory error"));
		}
		
		if (ft_strlen(trimmed) == 0 || trimmed[0] == '\0')
		{
			free(trimmed);
			i++;
			continue;
		}
		
		if (starts_with(trimmed, "NO ") && !no)
		{
out_cfg->textures.no = ft_strdup(trimmed + 3);
			no = 1;
		}
		else if (starts_with(trimmed, "SO ") && !so)
		{
out_cfg->textures.so = ft_strdup(trimmed + 3);
			so = 1;
		}
		else if (starts_with(trimmed, "EA ") && !ea)
		{
out_cfg->textures.ea = ft_strdup(trimmed + 3);
			ea = 1;
		}
		else if (starts_with(trimmed, "WE ") && !we)
		{
out_cfg->textures.we = ft_strdup(trimmed + 3);
			we = 1;
		}
		else if (starts_with(trimmed, "DO ")) /* Door texture */
		{
			out_cfg->textures.door = ft_strdup(trimmed + 3);
		}
		else if (starts_with(trimmed, "F ") && !f)
		{
			if (parse_color(trimmed + 2, &out_cfg->floor_color) != 0)
			{
				free(trimmed);
				free_lines(lines, count);
				return (1);
			}
			f = 1;
		}
		else if (starts_with(trimmed, "C ") && !c)
		{
			if (parse_color(trimmed + 2, &out_cfg->ceiling_color) != 0)
			{
				free(trimmed);
				free_lines(lines, count);
				return (1);
			}
			c = 1;
		}
		else if (ft_strchr(trimmed, '1') || ft_strchr(trimmed, '0') || 
				 ft_strchr(trimmed, 'N') || ft_strchr(trimmed, 'S') || 
				 ft_strchr(trimmed, 'E') || ft_strchr(trimmed, 'W') ||
				 ft_strchr(trimmed, 'D') || ft_strchr(trimmed, '2'))
		{
			map_start = i;
			free(trimmed);
			break;
		}
		
		free(trimmed);
		i++;
	}
	
	if (!no || !so || !ea || !we || !f || !c)
	{
		free_lines(lines, count);
		return (error_msg("parse: missing required elements"));
	}
	
	if (map_start == 0)
	{
		free_lines(lines, count);
		return (error_msg("parse: no map found"));
	}
	
	if (parse_map_bonus(lines, count, map_start, out_cfg) != 0)
	{
		free_lines(lines, count);
		return (1);
	}
	
	free_lines(lines, count);
	return (0);
}
