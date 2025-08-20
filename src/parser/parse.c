// Parser scaffolding: initialize config, scan header directives, detect map start.
#include "cub3d.h"

// Initialize all fields to safe/sentinel values so later validation can detect what's missing.
static void	init_config(t_config *cfg)
{
	// Reset texture paths
	cfg->textures.NO = NULL;
	cfg->textures.SO = NULL;
	cfg->textures.EA = NULL;
	cfg->textures.WE = NULL;
	// Mark colors as unset
	cfg->floor_color.r = -1;
	cfg->floor_color.g = -1;
	cfg->floor_color.b = -1;
	cfg->floor_color.value = -1;
	cfg->ceiling_color.r = -1;
	cfg->ceiling_color.g = -1;
	cfg->ceiling_color.b = -1;
	cfg->ceiling_color.value = -1;
	// Empty map defaults
	cfg->map.grid = NULL;
	cfg->map.width = 0;
	cfg->map.height = 0;
	cfg->map.player_x = 0;
	cfg->map.player_y = 0;
	cfg->map.player_angle = 0.0;
}

// Free any memory that may have been allocated during parsing.
// Safe to call on partially-filled configs and on failure paths.
void	free_config(t_config *cfg)
{
	if (!cfg)
		return ;
	// Release texture strings
	free(cfg->textures.NO);
	free(cfg->textures.SO);
	free(cfg->textures.EA);
	free(cfg->textures.WE);
	// Release map grid rows (if any)
	if (cfg->map.grid)
	{
		size_t i = 0;
		// Free each row one by one
		while (i < cfg->map.height)
		{
			free(cfg->map.grid[i]);
			++i;
		}
		free(cfg->map.grid);
	}
}

// (removed helper for brevity)

// (removed old placeholder parser to avoid unused warnings)

// Heuristic: a map line contains only spaces and {0,1,N,S,E,W},
// and has at least one of those non-space characters.
static int	is_map_line(const char *line)
{
	size_t i;
	int has_char = 0;

	i = 0;
	while (line[i])
	{
		// Read next character
		char c = line[i];
		i++;
		// Spaces are allowed and ignored
		if (c == ' ')
			continue ;
		// Valid map characters: walls, floor, and spawn markers
		if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			has_char = 1; // remember that we saw a map char
			continue ;
		}
		// Any other character disqualifies the line from being a map line
		return (0);
	}
	// Must contain at least one non-space valid map character
	return (has_char);
}

// Extract substring pointer after an identifier like "NO", skipping spaces.
static const char	*after_id(const char *trim, size_t id_len)
{
	size_t i;

	i = id_len;
	// Skip spaces following the identifier
	while (trim[i] == ' ' || trim[i] == '\t')
		i++;
	return (trim + i);
}

// Parse and store a texture directive (NO/SO/EA/WE).
static int	store_texture(t_textures *tx, const char *id, const char *path)
{
	char	**slot;

	slot = NULL;
	// Choose which field to fill based on the 2-letter id
	if (id[0] == 'N' && id[1] == 'O')
		slot = &tx->NO;
	else if (id[0] == 'S' && id[1] == 'O')
		slot = &tx->SO;
	else if (id[0] == 'E' && id[1] == 'A')
		slot = &tx->EA;
	else if (id[0] == 'W' && id[1] == 'E')
		slot = &tx->WE;
	// Reject unknown id or duplicate assignment
	if (!slot || *slot)
		return (error_msg("invalid or duplicate texture"));
	// Duplicate path into config
	*slot = ft_strdup(path);
	if (!*slot)
		return (error_msg("malloc failed"));
	return (0);
}

// Parse an integer 0..255 and advance index.
static int	read_u8(const char *s, size_t *i, int *out)
{
	size_t k;
	int val;

	// Skip leading spaces
	*i = skip_spaces(s, *i);
	k = *i;
	// Require at least one digit
	if (!is_digit((unsigned char)s[k]))
		return (error_msg("color: expected digit"));
	val = 0;
	// Accumulate integer value
	while (s[k] && is_digit((unsigned char)s[k]))
	{
		val = val * 10 + (s[k] - '0');
		k++;
	}
	// Clamp to 0..255
	if (val < 0 || val > 255)
		return (error_msg("color: out of range"));
	// Write back value and new index
	*out = val;
	*i = k;
	return (0);
}

// Parse F/C line: three numbers separated by commas and optional spaces.
static int	parse_rgb_triplet(const char *s, t_color *c)
{
	int r;
	int g;
	int b;
	size_t i;

	i = 0;
	if (read_u8(s, &i, &r) != 0)
		return (1);
	i = skip_spaces(s, i); // expect first comma
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &g) != 0)
		return (1);
	i = skip_spaces(s, i); // expect second comma
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &b) != 0)
		return (1);
	// Store channels and combined RGB value (0xRRGGBB)
	c->r = r;
	c->g = g;
	c->b = b;
	c->value = (r << 16) | (g << 8) | b;
	return (0);
}

// Parse header line and store textures/colors.
static int	apply_directive(char *line, t_config *cfg, int *header_done)
{
	char	*trim;
	const char	*val;

	// Trim whitespace to simplify matching
	trim = str_trim_spaces(line);
	if (!trim)
		return (error_msg("malloc failed"));
	// Skip empty lines
	if (trim[0] == '\0')
		return (free(trim), 0);
	if (starts_with(trim, "NO ") || starts_with(trim, "SO ") ||
		starts_with(trim, "EA ") || starts_with(trim, "WE "))
	{
		// Texture path after two-letter id
		val = after_id(trim, 2);
		if (*val == '\0')
			return (free(trim), error_msg("texture: missing path"));
		if (store_texture(&cfg->textures, trim, val) != 0)
			return (free(trim), 1);
		*header_done = 0; // still in header
		return (free(trim), 0);
	}
	if (starts_with(trim, "F "))
	{
		val = after_id(trim, 1); // floor color
		if (parse_rgb_triplet(val, &cfg->floor_color) != 0)
			return (free(trim), 1);
		*header_done = 0;
		return (free(trim), 0);
	}
	if (starts_with(trim, "C "))
	{
		val = after_id(trim, 1); // ceiling color
		if (parse_rgb_triplet(val, &cfg->ceiling_color) != 0)
			return (free(trim), 1);
		*header_done = 0;
		return (free(trim), 0);
	}
	// Not a directive: treat this line as the start of the map block
	*header_done = 1;
	return (free(trim), 0);
}

// Ensure all 6 directives are present exactly once.
static int	validate_header(const t_config *cfg)
{
	if (!cfg->textures.NO || !cfg->textures.SO)
		return (error_msg("missing NO/SO"));
	if (!cfg->textures.EA || !cfg->textures.WE)
		return (error_msg("missing EA/WE"));
	if (cfg->floor_color.value < 0)
		return (error_msg("missing floor color"));
	if (cfg->ceiling_color.value < 0)
		return (error_msg("missing ceiling color"));
	return (0);
}

// Count map lines and detect empty-line violations inside the map block.
static int	count_map_lines(char **lines, size_t start, size_t total, size_t *out)
{
	size_t i;
	size_t n;
	int seen_nonempty;

	i = start;
	n = 0;
	seen_nonempty = 0;
	while (i < total)
	{
		// Consider only non-empty lines part of the map block
		char *trim = str_trim_spaces(lines[i]);
		if (!trim)
			return (error_msg("malloc failed"));
		if (trim[0] == '\0' && seen_nonempty)
			return (free(trim), error_msg("empty line inside map"));
		if (trim[0] != '\0')
		{
			seen_nonempty = 1; // once we see content, empties are not allowed
			n++;
		}
		free(trim);
		i++;
	}
	*out = n;
	return (0);
}

// Find max row width for rectangularization.
static size_t	max_line_width(char **lines, size_t start, size_t total)
{
	size_t i;
	size_t max;
	size_t w;

	i = start;
	max = 0;
	while (i < total)
	{
		// Use raw line length; padding will be added later
		w = ft_strlen(lines[i]);
		if (w > max)
			max = w;
		i++;
	}
	return (max);
}

// Allocate rectangular grid and copy with space padding on the right.
static int	build_grid(t_map *map, char **lines, size_t start)
{
	size_t i;
	size_t h;
	size_t w;
	size_t j;

	h = map->height;
	w = map->width;
	map->grid = (char **)malloc(sizeof(char *) * h);
	if (!map->grid)
		return (error_msg("malloc failed"));
	i = 0;
	while (i < h)
	{
		// Allocate and fill row i
		map->grid[i] = (char *)malloc(w + 1);
		if (!map->grid[i])
		{
			// free rows allocated so far and reset
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
		j = 0;
		while (j < w)
		{
			// Copy char if within source line, else pad with space
			if (j < ft_strlen(lines[start + i]))
				map->grid[i][j] = lines[start + i][j];
			else
				map->grid[i][j] = ' ';
			j++;
		}
		map->grid[i][w] = '\0'; // NUL-terminate for convenience
		i++;
	}
	return (0);
}

// Detect a single spawn (N/S/E/W) and replace it by '0' in the grid.
static int	detect_spawn(t_map *map)
{
	size_t y;
	size_t x;
	int found;

	found = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			char c = map->grid[y][x]; // candidate cell
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (found)
					return (error_msg("multiple spawns"));
				map->player_x = x; // store spawn and orientation
				map->player_y = y;
				map->player_angle = (c == 'E') ? 0.0 : (c == 'N') ? 1.5708 : (c == 'W') ? 3.14159 : 4.71239;
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

// Validate every floor cell has 4-neighbors inside and not space.
static int	validate_closed(const t_map *map)
{
	size_t y;
	size_t x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->grid[y][x] == '0') // check only floor cells
			{
				// Floor cannot touch outer border
				if (y == 0 || x == 0 || y + 1 >= map->height || x + 1 >= map->width)
					return (error_msg("open map at border"));
				// Neighbor to a space also means open
				if (map->grid[y-1][x] == ' ' || map->grid[y+1][x] == ' ' ||
					map->grid[y][x-1] == ' ' || map->grid[y][x+1] == ' ')
					return (error_msg("open map near space"));
			}
			x++;
		}
		y++;
	}
	return (0);
}

// High-level pipeline
int	parse_cub_file(const char *path, t_config *out_cfg)
{
	char	**lines;
	size_t	count;
	int		header_done;
	size_t	idx;

	// Validate pointers
	if (!path || !out_cfg)
		return (error_msg("internal: invalid args"));
	// Prepare empty config
	init_config(out_cfg);
	// Load entire file into memory for a simple multi-pass parse
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	// Phase 1: scan header until map start
	header_done = 0;
	idx = 0;
	while (idx < count && !header_done)
	{
		// If line looks like a map, stop header scan
		if (is_map_line(lines[idx]))
			header_done = 1;
		// Otherwise try to apply as directive
		else if (apply_directive(lines[idx], out_cfg, &header_done) != 0)
			return (free_lines(lines, count), free_config(out_cfg), 1);
		// advance to next line while in header
		if (!header_done)
			++idx;
	}
	// Validate header and ensure map exists
	if (validate_header(out_cfg) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	if (idx >= count)
		return (free_lines(lines, count), free_config(out_cfg), error_msg("missing map"));
	// Phase 2: extract/rectangularize map
	if (count_map_lines(lines, idx, count, &out_cfg->map.height) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	out_cfg->map.width = max_line_width(lines, idx, count);
	if (out_cfg->map.width == 0 || out_cfg->map.height == 0)
		return (free_lines(lines, count), free_config(out_cfg), error_msg("empty map"));
	if (build_grid(&out_cfg->map, lines, idx) != 0)
		return (free_lines(lines, count), free_config(out_cfg), 1);
	free_lines(lines, count);
	// Phase 3: spawn and closure validation
	if (detect_spawn(&out_cfg->map) != 0)
		return (free_config(out_cfg), 1);
	if (validate_closed(&out_cfg->map) != 0)
		return (free_config(out_cfg), 1);
	return (0);
}


