/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:05:12 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 21:05:12 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

# include "cub3d.h"

/* Helper structs for parameter grouping */
typedef struct s_parse_context
{
	char			**lines;
	size_t			count;
	t_config		*cfg;
	t_parse_flags	*flags;
	size_t			*map_start;
}	t_parse_context;

typedef struct s_char_position
{
	char	c;
	size_t	i;
	size_t	j;
	int		*player_count;
}	t_char_position;

typedef struct s_header_context
{
	char			*trimmed;
	char			**lines;
	size_t			count;
	t_config		*out_cfg;
	t_parse_flags	*flags;
}	t_header_context;

/* Function declarations */

/* Config functions */
void			init_config(t_config *cfg);
void			free_config(t_config *cfg);

/* Color functions */
int				parse_color(const char *line, t_color *out_color);
t_parse_flags	init_flags(void);

/* Sprite and door functions */
int				add_sprite(t_config *cfg, int x, int y, char type);
int				add_door(t_config *cfg, int x, int y);

/* Map functions */
size_t			get_max_width(char **lines, size_t start, size_t count);
int				fill_map_grid(t_config *cfg, char **lines, size_t start,
					size_t count);
int				parse_map_cells(t_config *cfg, int *player_count);
int				parse_map(char **lines, size_t count, size_t map_start,
					t_config *cfg);
int				validate_map(const t_map *map);
int				handle_map_char(t_config *cfg, t_char_position *pos);
int				validate_map_borders(char **map_lines, size_t height);
int				validate_all_map(char **map_lines, size_t height);

/* Map utility functions */
int				handle_player_char(t_config *cfg, t_char_position *pos);
int				handle_door_char(t_config *cfg, size_t i, size_t j);
int				handle_sprite_char(t_config *cfg, char c, size_t i,
					size_t j);

/* Header functions */
int				parse_headers(t_parse_context *ctx);
int				parse_header_line(const char *trimmed, t_config *out_cfg,
					t_parse_flags *flags);
int				validate_required_elements(const t_parse_flags *flags);
int				handle_empty_line(char *trimmed, size_t *map_start);
int				process_header_line(t_header_context *ctx);
int				process_single_header(t_parse_context *ctx, char *trimmed);
int				parse_single_line(t_parse_context *ctx);

/* Texture functions */
int				validate_texture_file(const char *path);
int				handle_wall_textures(const char *trimmed, t_config *out_cfg,
					t_parse_flags *flags);
int				handle_door_texture(const char *trimmed, t_config *out_cfg);
int				handle_texture_header(const char *trimmed, t_config *out_cfg,
					t_parse_flags *flags);
int				handle_color_header(const char *trimmed, t_config *out_cfg,
					t_parse_flags *flags);
int				is_map_start(const char *trimmed);

/* Grid allocation functions */
int				allocate_map_row(t_config *cfg, char **lines, size_t line_idx,
					size_t row_idx);

/* Validation functions */
int				validate_map_characters(char **map_lines, size_t height);
int				is_valid_map_position(char **map_lines, size_t height,
					size_t row, size_t col);
int				validate_and_parse_map(t_parse_context *ctx,
					size_t map_start);

#endif
