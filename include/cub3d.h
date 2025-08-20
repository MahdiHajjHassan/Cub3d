#ifndef CUB3D_H
# define CUB3D_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft/libft.h"

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	value;
}t_color;

typedef struct s_textures
{
	char	*NO;
	char	*SO;
	char	*EA;
	char	*WE;
}t_textures;

typedef struct s_map
{
	char	**grid;
	size_t	width;
	size_t	height;
	size_t	player_x;
	size_t	player_y;
	double	player_angle;
}t_map;

typedef struct s_config
{
	t_textures	textures;
	t_color		floor_color;
	t_color		ceiling_color;
	t_map		map;
}t_config;

int	run_game(const t_config *cfg);

/* Parser entrypoints */
int	parse_cub_file(const char *path, t_config *out_cfg);
void	free_config(t_config *cfg);

/* Error helper (prints to stderr, returns 1) */
int	error_msg(const char *msg);

/* File utils */
int	read_all_lines(const char *path, char ***out_lines, size_t *out_count);
void	free_lines(char **lines, size_t count);

/* String utils */
int	is_space(int c);
int	is_digit(int c);
size_t	skip_spaces(const char *s, size_t i);
size_t	skip_digits(const char *s, size_t i);
char	*str_trim_spaces(const char *s);
int	starts_with(const char *s, const char *prefix);

#endif


