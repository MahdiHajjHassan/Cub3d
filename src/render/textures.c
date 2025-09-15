/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/15 14:17:27 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

static int	load_wall_texture(t_game *g, const char *path, int index);
static int	load_sprite_textures(t_game *g);
static int	load_all_wall_textures(t_game *g, const t_config *cfg);

int	textures_load(t_game *g, const t_config *cfg)
{
	if (load_all_wall_textures(g, cfg) != 0)
		return (1);
	if (load_sprite_textures(g) != 0)
		return (error_msg("textures: failed to load sprite textures"));
	g->ceil_color = mlx_get_color_value(g->mlx, cfg->ceiling_color.value);
	g->floor_color = mlx_get_color_value(g->mlx, cfg->floor_color.value);
	return (0);
}

static int	load_all_wall_textures(t_game *g, const t_config *cfg)
{
	if (load_wall_texture(g, cfg->textures.no, TEX_NO) != 0)
		if (load_wall_texture(g, NULL, TEX_NO) != 0)
			return (error_msg("textures: failed NO"));
	if (load_wall_texture(g, cfg->textures.so, TEX_SO) != 0)
		if (load_wall_texture(g, NULL, TEX_SO) != 0)
			return (error_msg("textures: failed SO"));
	if (load_wall_texture(g, cfg->textures.ea, TEX_EA) != 0)
		if (load_wall_texture(g, NULL, TEX_EA) != 0)
			return (error_msg("textures: failed EA"));
	if (load_wall_texture(g, cfg->textures.we, TEX_WE) != 0)
		if (load_wall_texture(g, NULL, TEX_WE) != 0)
			return (error_msg("textures: failed WE"));
	if (cfg->textures.door)
	{
		if (load_wall_texture(g, cfg->textures.door, TEX_DOOR) != 0)
			if (load_wall_texture(g, NULL, TEX_DOOR) != 0)
				return (error_msg("textures: failed door"));
	}
	else
		if (load_wall_texture(g, NULL, TEX_DOOR) != 0)
			return (error_msg("textures: failed door gen"));
	return (0);
}

static int	load_wall_texture(t_game *g, const char *path, int index)
{
	const char	*fallback_paths[5] = {
		"assets/wall-enhanced_1.xpm",
		"assets/wall-enhanced_1.xpm",
		"assets/wall-enhanced_1.xpm",
		"assets/wall-enhanced_1.xpm",
		"assets/door-enhanced_1.xpm"
	};

	if (index >= TEX_COUNT)
		return (error_msg("textures: invalid texture index"));
	// Force all wall faces to use wall-enhanced_1.xpm
	if (index >= TEX_NO && index <= TEX_WE)
		path = "assets/wall-enhanced_1.xpm";
	// Force door texture to use door-enhanced_1.xpm
	else if (index == TEX_DOOR)
		path = "assets/door-enhanced_1.xpm";
	else if (!path || !*path)
		path = fallback_paths[index];
	g->tex[index].img = mlx_xpm_file_to_image(g->mlx, (char *)path,
			&g->tex[index].width, &g->tex[index].height);
	if (!g->tex[index].img)
		return (1);
	g->tex[index].data = mlx_get_data_addr(g->tex[index].img,
			&g->tex[index].bpp, &g->tex[index].line_len, &g->tex[index].endian);
	if (!g->tex[index].data)
	{
		mlx_destroy_image(g->mlx, g->tex[index].img);
		return (1);
	}
	return (0);
}

static int	load_sprite_textures(t_game *g)
{
	const char	*paths[2] = {"assets/sprite1-back_1.xpm",
		"assets/sprite2-back_1.xpm"};
	int			i;

	i = 0;
	while (i < 2)
	{
		g->tex[SPRITE_TEX_START + i].img = mlx_xpm_file_to_image(g->mlx,
				(char *)paths[i], &g->tex[SPRITE_TEX_START + i].width,
				&g->tex[SPRITE_TEX_START + i].height);
		if (!g->tex[SPRITE_TEX_START + i].img)
			return (error_msg("textures: sprite xpm load failed"));
		g->tex[SPRITE_TEX_START + i].data = mlx_get_data_addr(
				g->tex[SPRITE_TEX_START + i].img,
				&g->tex[SPRITE_TEX_START + i].bpp,
				&g->tex[SPRITE_TEX_START + i].line_len,
				&g->tex[SPRITE_TEX_START + i].endian);
		if (!g->tex[SPRITE_TEX_START + i].data)
		{
			mlx_destroy_image(g->mlx, g->tex[SPRITE_TEX_START + i].img);
			return (error_msg("textures: sprite data addr failed"));
		}
		i++;
	}
	return (0);
}

void	textures_destroy(t_game *g)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (g->mlx && g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i] = (t_img){0};
		i++;
	}
	i = SPRITE_TEX_START;
	while (i < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		if (g->mlx && g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i] = (t_img){0};
		i++;
	}
}
