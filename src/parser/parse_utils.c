/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:12:30 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 19:12:30 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

int	is_map_line(const char *line)
{
	size_t	i;
	int		has_char;
	char	c;

	i = 0;
	has_char = 0;
	while (line[i])
	{
		c = line[i];
		if (c != ' ' && c != '0' && c != '1' && c != 'N'
			&& c != 'S' && c != 'E' && c != 'W')
			return (0);
		if (c != ' ')
			has_char = 1;
		i++;
	}
	return (has_char);
}

static const char	*after_id(const char *trim, size_t id_len)
{
	size_t	i;

	i = id_len;
	while (trim[i] == ' ' || trim[i] == '\t')
		i++;
	return (trim + i);
}

static int	store_texture(t_textures *tx, const char *id, const char *path)
{
	char	**slot;

	slot = NULL;
	if (id[0] == 'N' && id[1] == 'O')
		slot = &tx->no;
	else if (id[0] == 'S' && id[1] == 'O')
		slot = &tx->so;
	else if (id[0] == 'E' && id[1] == 'A')
		slot = &tx->ea;
	else if (id[0] == 'W' && id[1] == 'E')
		slot = &tx->we;
	if (!slot || *slot)
		return (error_msg("invalid or duplicate texture"));
	*slot = ft_strdup(path);
	if (!*slot)
		return (error_msg("malloc failed"));
	return (0);
}

static int	read_u8(const char *s, size_t *i, int *out)
{
	size_t	k;
	int		val;

	*i = skip_spaces(s, *i);
	k = *i;
	if (!is_digit((unsigned char)s[k]))
		return (error_msg("color: expected digit"));
	val = 0;
	while (s[k] && is_digit((unsigned char)s[k]))
	{
		val = val * 10 + (s[k] - '0');
		k++;
	}
	if (val < 0 || val > 255)
		return (error_msg("color: out of range"));
	*out = val;
	*i = k;
	return (0);
}

static int	parse_rgb_triplet(const char *s, t_color *c)
{
	int		r;
	int		g;
	int		b;
	size_t	i;

	i = 0;
	if (read_u8(s, &i, &r) != 0)
		return (1);
	i = skip_spaces(s, i);
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &g) != 0)
		return (1);
	i = skip_spaces(s, i);
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &b) != 0)
		return (1);
	c->r = r;
	c->g = g;
	c->b = b;
	c->value = (r << 16) | (g << 8) | b;
	return (0);
}

int	apply_directive(char *line, t_config *cfg, int *header_done)
{
	char		*trim;
	const char	*val;

	trim = str_trim_spaces(line);
	if (!trim)
		return (error_msg("malloc failed"));
	if (trim[0] == '\0')
		return (free(trim), 0);
	if (starts_with(trim, "NO ") || starts_with(trim, "SO ")
		|| starts_with(trim, "EA ") || starts_with(trim, "WE "))
	{
		val = after_id(trim, 2);
		if (*val == '\0')
			return (free(trim), error_msg("texture: missing path"));
		if (store_texture(&cfg->textures, trim, val) != 0)
			return (free(trim), 1);
		*header_done = 0;
		return (free(trim), 0);
	}
	if (starts_with(trim, "F "))
	{
		val = after_id(trim, 1);
		if (parse_rgb_triplet(val, &cfg->floor_color) != 0)
			return (free(trim), 1);
		*header_done = 0;
		return (free(trim), 0);
	}
	if (starts_with(trim, "C "))
	{
		val = after_id(trim, 1);
		if (parse_rgb_triplet(val, &cfg->ceiling_color) != 0)
			return (free(trim), 1);
		*header_done = 0;
		return (free(trim), 0);
	}
	*header_done = 1;
	return (free(trim), 0);
}

int	count_map_lines(char **lines, size_t start, size_t total, size_t *out)
{
	size_t	i;
	size_t	n;
	int		seen;
	char	*trim;

	i = start;
	n = 0;
	seen = 0;
	while (i < total)
	{
		trim = str_trim_spaces(lines[i]);
		if (!trim)
			return (error_msg("malloc failed"));
		if (trim[0] == '\0' && seen)
			return (free(trim), error_msg("empty line inside map"));
		if (trim[0] != '\0')
		{
			seen = 1;
			n++;
		}
		free(trim);
		i++;
	}
	*out = n;
	return (0);
}

size_t	max_line_width(char **lines, size_t start, size_t total)
{
	size_t	i;
	size_t	max;
	size_t	w;

	i = start;
	max = 0;
	while (i < total)
	{
		w = ft_strlen(lines[i]);
		if (w > max)
			max = w;
		i++;
	}
	return (max);
}

int	build_grid(t_map *map, char **lines, size_t start)
{
	size_t	i;
	size_t	j;
	size_t	h;
	size_t	w;
	size_t	src_len;

	h = map->height;
	w = map->width;
	map->grid = (char **)malloc(sizeof(char *) * h);
	if (!map->grid)
		return (error_msg("malloc failed"));
	i = 0;
	while (i < h)
	{
		map->grid[i] = (char *)malloc(w + 1);
		if (!map->grid[i])
		{
			while (i > 0)
			{
				free(map->grid[i - 1]);
				i--;
			}
			free(map->grid);
			map->grid = NULL;
			map->height = 0;
			map->width = 0;
			return (error_msg("malloc failed"));
		}
		src_len = ft_strlen(lines[start + i]);
		j = 0;
		while (j < w)
		{
			if (j < src_len)
				map->grid[i][j] = lines[start + i][j];
			else
				map->grid[i][j] = ' ';
			j++;
		}
		map->grid[i][w] = '\0';
		i++;
	}
	return (0);
}

int	detect_spawn(t_map *map)
{
	size_t	y;
	size_t	x;
	int		found;
	char	c;

	found = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			c = map->grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (found)
					return (error_msg("multiple spawns"));
				map->player_x = x;
				map->player_y = y;
				if (c == 'E')
					map->player_angle = 0.0;
				else if (c == 'N')
					map->player_angle = 1.5708;
				else if (c == 'W')
					map->player_angle = 3.14159;
				else
					map->player_angle = 4.71239;
				map->grid[y][x] = '0';
				found = 1;
			}
			x++;
		}
		y++;
	}
	if (!found)
		return (error_msg("missing spawn"));
	return (0);
}

int	validate_closed(const t_map *map)
{
	size_t	y;
	size_t	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->grid[y][x] == '0')
			{
				if (y == 0 || x == 0 || y + 1 >= map->height
					|| x + 1 >= map->width)
					return (error_msg("open map at border"));
				if (map->grid[y - 1][x] == ' '
					|| map->grid[y + 1][x] == ' '
					|| map->grid[y][x - 1] == ' '
					|| map->grid[y][x + 1] == ' ')
					return (error_msg("open map near space"));
			}
			x++;
		}
		y++;
	}
	return (0);
}

int	find_map_start_and_apply(char **lines, size_t count,
								t_config *cfg, size_t *idx)
{
	size_t	i;
	int		header_done;

	i = *idx;
	header_done = 0;
	while (i < count && !header_done)
	{
		if (is_map_line(lines[i]))
			header_done = 1;
		else if (apply_directive(lines[i], cfg, &header_done) != 0)
			return (1);
		if (!header_done)
			i++;
	}
	*idx = i;
	return (0);
}
