/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:39:19 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"
# include <X11/Xlib.h>

typedef struct s_xvar
{
	Display	*display;
	Window	root;
	int		screen;
}	t_xvar;

typedef struct s_win_list
{
	Window		window;
	void		*next;
}	t_win_list;

typedef struct s_ray_data
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	bool	hit_door;
}	t_ray_data;

typedef struct s_wall_data
{
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_id;
	double	wall_x;
}	t_wall_data;

typedef struct s_texture_data
{
	t_img	*tx;
	int		tex_x;
	double	step;
	double	tex_pos;
	int		bpp;
	int		tbpp;
}	t_texture_data;

typedef struct s_door_data
{
	char	cell;
	int		i;
	bool	door_is_open;
}	t_door_data;

typedef struct s_rect_data
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		color;
}	t_rect_data;

typedef struct s_stripe_data
{
	int		x;
	int		tex_id;
	int		draw_start;
	int		draw_end;
	int		line_height;
	double	wall_x;
}	t_stripe_data;

typedef struct s_pixel_data
{
	int		x;
	int		y;
	int		color;
}	t_pixel_data;

typedef struct s_minimap_rect_data
{
	int		x;
	int		y;
	int		size;
	int		color;
}	t_minimap_rect_data;

typedef struct s_line_data
{
	int		start_x;
	int		start_y;
	int		dx;
	int		dy;
	int		steps;
}	t_line_data;

typedef struct s_sprite_bounds
{
	int		draw_start_x;
	int		draw_end_x;
	int		draw_start_y;
	int		draw_end_y;
}	t_sprite_bounds;

typedef struct s_sprite_draw_data
{
	int		stripe;
	int		draw_start_y;
	int		draw_end_y;
	int		sprite_height;
	int		sprite_screen_x;
	int		sprite_width;
}	t_sprite_draw_data;

typedef struct s_sprite_pixel_data
{
	int		tex_x;
	int		tex_y;
	int		y;
	double	transform_y;
}	t_sprite_pixel_data;

/* Function prototypes for render module */
int		frame_init(t_game *g);
void	frame_destroy(t_game *g);
void	render_frame(t_game *g);
void	fill_rect(t_img *img, t_rect_data rect);
void	draw_vertical_stripe(t_game *g, t_stripe_data stripe);
void	init_ray(t_game *g, t_ray_data *ray, int x);
void	setup_ray_steps(t_game *g, t_ray_data *ray);
void	perform_dda(t_game *g, t_ray_data *ray, t_door_data *door);
void	calculate_wall(t_game *g, t_ray_data *ray, t_wall_data *wall);
void	setup_wall_texture(t_game *g, t_ray_data *ray, t_wall_data *wall);

/* Minimap functions */
int		init_minimap(t_game *g);
int		setup_minimap_image(t_game *g);
void	destroy_minimap(t_game *g);
void	draw_minimap_pixel(t_img *img, t_pixel_data pixel);
void	draw_minimap_rect(t_img *img, t_minimap_rect_data rect);
void	render_minimap(t_game *g);
void	draw_player_on_minimap(t_game *g);
void	draw_line_on_minimap(t_game *g, int start_x, int start_y);
void	draw_direction_line(t_game *g, t_line_data line);
void	clear_minimap_background(t_game *g);
void	draw_map_cell(t_game *g, int x, int y);

/* Movement functions */
void	move_axis(t_game *g, double nx, double ny, double buf);
void	handle_forward_movement(t_game *g);
void	handle_backward_movement(t_game *g);
void	handle_strafe_movement(t_game *g);
void	handle_rotation_movement(t_game *g);

/* Game functions */
int		display_available(void);
void	init_game_from_cfg(t_game *g, const t_config *cfg);
void	init_game_controls(t_game *g);
void	init_game_speeds(t_game *g);
void	init_game_colors(t_game *g, const t_config *cfg);
void	init_game_structures(t_game *g);
int		setup_mlx_window(t_game *g);
void	center_window_on_screen(void *mlx_ptr, void *win_ptr);
int		setup_game_resources(t_game *g, const t_config *cfg);
void	cleanup_mlx_window(t_game *g);
void	cleanup_game_partial(t_game *g);
void	cleanup_game_full(t_game *g);
void	cleanup_game_complete(t_game *g);

/* External function declarations */
int		textures_load(t_game *g, const t_config *cfg);
void	textures_destroy(t_game *g);
int		on_key_press(int keycode, void *param);
int		on_key_release(int keycode, void *param);
int		on_destroy(void *param);
int		on_mouse_move(int x, int y, void *param);
int		game_loop(void *param);

/* Sprite functions */
int		iabs(int value);
int		check_transparency(char *tex_pixel);
void	sort_sprites_by_distance(t_game *g);
double	get_time_delta(struct timeval *last_time);
void	update_sprites(t_game *g);
void	render_sprites(t_game *g);
void	render_single_sprite(t_game *g, t_sprite *sprite);
void	draw_sprite_column(t_game *g, t_sprite *sprite,
			t_sprite_draw_data data, t_sprite_pixel_data *pixel);
void	draw_sprite_pixel(t_game *g, t_img *tex,
			t_sprite_draw_data data, t_sprite_pixel_data *pixel);

#endif
