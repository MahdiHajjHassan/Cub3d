/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:22:01 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 18:22:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d_bonus.h"
#include "mlx.h"

static int load_wall_texture(t_game *g, const char *path, int index);
static int load_sprite_textures(t_game *g);
static int generate_cyber_texture(t_game *g, int index);

int textures_load(t_game *g, const t_config *cfg)
{
	/* Load wall textures (fallback to procedural cyber theme if load fails) */
	if (load_wall_texture(g, cfg->textures.no, TEX_NO) != 0)
		if (generate_cyber_texture(g, TEX_NO) != 0)
			return (error_msg("textures: failed NO"));
	if (load_wall_texture(g, cfg->textures.so, TEX_SO) != 0)
		if (generate_cyber_texture(g, TEX_SO) != 0)
			return (error_msg("textures: failed SO"));
	if (load_wall_texture(g, cfg->textures.ea, TEX_EA) != 0)
		if (generate_cyber_texture(g, TEX_EA) != 0)
			return (error_msg("textures: failed EA"));
	if (load_wall_texture(g, cfg->textures.we, TEX_WE) != 0)
		if (generate_cyber_texture(g, TEX_WE) != 0)
			return (error_msg("textures: failed WE"));
	
	/* Door: try load else generate cyber door */
	if (cfg->textures.door)
	{
		if (load_wall_texture(g, cfg->textures.door, TEX_DOOR) != 0)
			if (generate_cyber_texture(g, TEX_DOOR) != 0)
				return (error_msg("textures: failed door"));
	}
	else
	{
		if (generate_cyber_texture(g, TEX_DOOR) != 0)
			return (error_msg("textures: failed door gen"));
	}
	
	/* Load sprite textures */
	if (load_sprite_textures(g) != 0)
		return (error_msg("textures: failed to load sprite textures"));
	
	/* Convert colors for MLX */
	g->ceil_color = mlx_get_color_value(g->mlx, cfg->ceiling_color.value);
	g->floor_color = mlx_get_color_value(g->mlx, cfg->floor_color.value);
	
	return (0);
}

static int load_wall_texture(t_game *g, const char *path, int index)
{
	if (index >= TEX_COUNT)
		return (error_msg("textures: invalid texture index"));
	if (!path || !*path)
		return (1);
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

static int generate_cyber_texture(t_game *g, int index)
{
	int w = 64, h = 64;
	int x, y;
	int bpp;
	int bg = 0x0A0A12; /* dark background */
	int grid = 0x00FFCC; /* neon grid default */
	int trace = 0x00FF66; /* bright trace default */
	/* Per-face palette */
	if (index == TEX_NO) { grid = 0x00FF99; trace = 0x00FF33; }
	else if (index == TEX_SO) { grid = 0xFF3300; trace = 0xFFFF00; }
	else if (index == TEX_EA) { grid = 0x00AAFF; trace = 0x00FFFF; }
	else if (index == TEX_WE) { grid = 0xFF8800; trace = 0xFFAA00; }
	else if (index == TEX_DOOR) { grid = 0x00FF00; trace = 0x009900; bg = 0x111122; }

	g->tex[index].img = mlx_new_image(g->mlx, w, h);
	if (!g->tex[index].img)
		return (error_msg("textures: gen image failed"));
	g->tex[index].data = mlx_get_data_addr(g->tex[index].img,
		&g->tex[index].bpp, &g->tex[index].line_len, &g->tex[index].endian);
	if (!g->tex[index].data)
	{
		mlx_destroy_image(g->mlx, g->tex[index].img);
		return (error_msg("textures: gen data addr failed"));
	}
	g->tex[index].width = w;
	g->tex[index].height = h;
	bpp = g->tex[index].bpp / 8;
	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
		{
			int color = bg;
			/* grid lines every 8 px */
			if ((x % 8) == 0 || (y % 8) == 0)
				color = grid;
			/* diagonal traces */
			if (((x + y) & 15) == 0 || (((x - y) & 15) == 0))
				color = trace;
			/* door center panel */
			if (index == TEX_DOOR)
			{
				if (x > 18 && x < 46 && y > 12 && y < 40)
					color = (y % 6 == 0 || x % 6 == 0) ? grid : 0x001A10;
				/* two vertical LED stripes */
				if ((x == 22 || x == 42) && y > 16 && y < 36)
					color = trace;
			}
			char *p = g->tex[index].data + y * g->tex[index].line_len + x * bpp;
			p[0] = (char)(color & 0xFF);
			p[1] = (char)((color >> 8) & 0xFF);
			p[2] = (char)((color >> 16) & 0xFF);
			p[3] = 0;
		}
	}
	return (0);
}

static int load_sprite_textures(t_game *g)
{
	int tex_index = SPRITE_TEX_START;
	int sprite_id = 0;
	int frame = 0;
	
	/* Create default sprite texture if no sprites are loaded */
	/* For simplicity, we'll create a basic colored square as default sprite */
	while (sprite_id < 1 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		frame = 0;
		while (frame < 4 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
		{
			g->tex[tex_index].img = mlx_new_image(g->mlx, 64, 64);
			if (!g->tex[tex_index].img)
				return (error_msg("textures: sprite image creation failed"));
			
			g->tex[tex_index].data = mlx_get_data_addr(g->tex[tex_index].img,
				&g->tex[tex_index].bpp, &g->tex[tex_index].line_len, &g->tex[tex_index].endian);
			if (!g->tex[tex_index].data)
			{
				mlx_destroy_image(g->mlx, g->tex[tex_index].img);
				return (error_msg("textures: sprite data addr failed"));
			}
			
			g->tex[tex_index].width = 64;
			g->tex[tex_index].height = 64;
			
			/* Fill with different colors per frame for animation (cyber green tones) */
			int color = 0x00FF00; /* Green base */
			if (frame == 1) color = 0x33FF33;
			else if (frame == 2) color = 0x66FF66;
			else if (frame == 3) color = 0x99FF99;
			
			/* Fill the texture with the color */
			int y = 0;
			while (y < 64)
			{
				int x = 0;
				while (x < 64)
				{
					int bpp = g->tex[tex_index].bpp / 8;
					if (bpp >= 4)
					{
						char *pixel = g->tex[tex_index].data + y * g->tex[tex_index].line_len + x * bpp;
						
						/* Create a simple sprite pattern */
						if ((x >= 16 && x < 48) && (y >= 16 && y < 48))
						{
							pixel[0] = (char)(color & 0xFF);
							pixel[1] = (char)((color >> 8) & 0xFF);
							pixel[2] = (char)((color >> 16) & 0xFF);
							pixel[3] = 0;
						}
						else
						{
							/* Transparent pixels */
							pixel[0] = 0;
							pixel[1] = 0;
							pixel[2] = 0;
							pixel[3] = 0;
						}
					}
					x++;
				}
				y++;
			}
			
			tex_index++;
			frame++;
		}
		sprite_id++;
	}
	
	return (0);
}

void textures_destroy(t_game *g)
{
	int i;
	
	/* Destroy wall textures */
	i = 0;
	while (i < TEX_COUNT)
	{
		if (g->mlx && g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i] = (t_img){0};
		i++;
	}
	
	/* Destroy sprite textures */
	i = SPRITE_TEX_START;
	while (i < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		if (g->mlx && g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i] = (t_img){0};
		i++;
	}
}
