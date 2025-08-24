/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:10:19 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 19:05:00 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include <sys/time.h>
# include "libft/libft.h"

# define WIN_W 1280
# define WIN_H 800

# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_SPACE 32

# define MOUSE_LEFT 1
# define MOUSE_RIGHT 3

# define TEX_NO 0
# define TEX_SO 1
# define TEX_EA 2
# define TEX_WE 3
# define TEX_DOOR 4
# define TEX_COUNT 5

# define SPRITE_TEX_START 5
# define MAX_SPRITES 100
# define MAX_SPRITE_FRAMES 8

# define MINIMAP_SIZE 200
# define MINIMAP_MARGIN 10
# define MINIMAP_SCALE 8

# define TEX_TOTAL 805

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	value;
}	t_color;

typedef struct s_textures
{
	char	*no;
	char	*so;
	char	*ea;
	char	*we;
	char	*door;
	char	**sprites;
	int		sprite_count;
}	t_textures;

typedef struct s_door
{
	int		x;
	int		y;
	bool	is_open;
	double	open_timer;
}	t_door;

typedef struct s_sprite
{
	double	x;
	double	y;
	int		texture_id;
	int		frame_count;
	int		current_frame;
	double	frame_timer;
	bool	animated;
	double	distance;
}	t_sprite;

typedef struct s_map
{
	char		**grid;
	size_t		width;
	size_t		height;
	size_t		player_x;
	size_t		player_y;
	double		player_angle;
	t_door		*doors;
	int			door_count;
	t_sprite	*sprites;
	int			sprite_count;
}	t_map;

typedef struct s_config
{
	t_textures	textures;
	t_color		floor_color;
	t_color		ceiling_color;
	t_map		map;
}	t_config;

typedef struct s_img
{
	void	*img;
	char	*data;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
	int	space;
}	t_keys;

typedef struct s_mouse
{
	int		x;
	int		y;
	int		last_x;
	int		last_y;
	bool	captured;
}	t_mouse;

typedef struct s_minimap
{
	int		size;
	int		margin;
	int		scale;
	t_img	img;
}	t_minimap;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_img	frame;
	t_img	tex[TEX_TOTAL];
	int	ceil_color;
	int	floor_color;
	const t_map	*map;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	t_keys	keys;
	t_mouse	mouse;
	double	move_speed;
	double	rot_speed;
	t_minimap	minimap;
	struct timeval	last_time;
	double	frame_time;
}	t_game;

int		run_game(const t_config *cfg);
int		parse_cub_file(const char *path, t_config *out_cfg);
void	free_config(t_config *cfg);
int		error_msg(const char *msg);
int		read_all_lines(const char *path, char ***out_lines, size_t *out_count);
void	free_lines(char **lines, size_t count);
int		is_space(int c);
int		is_digit(int c);
size_t	skip_spaces(const char *s, size_t i);
size_t	skip_digits(const char *s, size_t i);
char	*str_trim_spaces(const char *s);
int		starts_with(const char *s, const char *prefix);
void	update_sprites(t_game *g);
void	render_sprites(t_game *g);
void	render_minimap(t_game *g);
int		init_minimap(t_game *g);
void	destroy_minimap(t_game *g);
void	handle_door_interaction(t_game *g);
void	update_doors(t_game *g);
void	render_frame_bonus(t_game *g);
double	get_time_delta(struct timeval *last_time);

#endif
