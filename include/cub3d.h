#ifndef CUB3D_H
# define CUB3D_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include "libft/libft.h"

/* Window size (can be tuned) */
# define WIN_W 800
# define WIN_H 600

/* X11 keycodes (Linux) */
# define KEY_ESC   65307
# define KEY_LEFT  65361
# define KEY_RIGHT 65363
# define KEY_W     119
# define KEY_A     97
# define KEY_S     115
# define KEY_D     100

/* Texture indices */
# define TEX_NO 0
# define TEX_SO 1
# define TEX_EA 2
# define TEX_WE 3
# define TEX_COUNT 4

/* Colors and textures parsed from file */
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	value; /* 0xRRGGBB */
}t_color;

typedef struct s_textures
{
	char	*NO;
	char	*SO;
	char	*EA;
	char	*WE;
}t_textures;

/* Map representation */
typedef struct s_map
{
	char	**grid;   /* height x width chars */
	size_t	width;
	size_t	height;
	size_t	player_x;    /* cell coordinates */
	size_t	player_y;
	double	player_angle; /* radians, 0=east, pi/2=north */
}t_map;

/* Full configuration */
typedef struct s_config
{
	t_textures	textures;
	t_color		floor_color;
	t_color		ceiling_color;
	t_map		map;
}t_config;

/* Lightweight image wrapper for MLX images */
typedef struct s_img
{
	void	*img;       /* MLX image handle */
	char	*data;      /* pixel buffer */
	int		bpp;       /* bits per pixel */
	int		line_len;  /* bytes per row */
	int		endian;    /* endian flag */
	int		width;
	int		height;
}t_img;

/* Input state */
typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}t_keys;

/* Main runtime game state */
typedef struct s_game
{
	/* MLX */
	void	*mlx;
	void	*win;
	t_img	frame;            /* off-screen frame buffer */
	t_img	tex[TEX_COUNT];    /* wall textures */
	int		ceil_color;       /* converted via mlx_get_color_value */
	int		floor_color;
	/* World */
	const t_map	*map;          /* borrow from cfg */
	double	pos_x;            /* player position in world units */
	double	pos_y;
	double	dir_x;            /* view direction */
	double	dir_y;
	double	plane_x;          /* camera plane (perpendicular to dir) */
	double	plane_y;
	/* Input */
	t_keys	keys;
	/* Timing-independent speeds */
	double	move_speed;
	double	rot_speed;
}t_game;

/* Entry point to start the game loop */
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
