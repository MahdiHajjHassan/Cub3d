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
	char		**lines;
	size_t		count;
	t_config	*cfg;
	t_parse_flags	*flags;
	size_t		*map_start;
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
	char		*trimmed;
	char		**lines;
	size_t		count;
	t_config	*out_cfg;
	t_parse_flags	*flags;
}	t_header_context;

/* Header parsing */
int		apply_directive(char *line, t_config *cfg, int *header_done);
int		parse_rgb_triplet(const char *s, t_color *c);

/* Map parsing */
int		is_map_line(const char *line);
int		count_map_lines(char **lines, size_t start, size_t total, size_t *out);
size_t	max_line_width(char **lines, size_t start, size_t total);
int		build_grid(t_map *map, char **lines, size_t start);
int		detect_spawn(t_map *map);
int		validate_closed(const t_map *map);
int		find_map_start_and_apply(char **lines, size_t count,
								t_config *cfg, size_t *idx);

#endif
