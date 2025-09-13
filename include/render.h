/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 17:41:17 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"

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

#endif
