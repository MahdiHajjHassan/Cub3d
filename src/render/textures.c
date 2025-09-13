/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 01:37:57 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

	static int	load_wall_texture(t_game *g, const char *path, int index);
	static int	load_sprite_textures(t_game *g);
	static int	generate_cyber_texture(t_game *g, int index);
	static void	put_pixel(t_img *tex, int x, int y, int color, int w, int h);

int	textures_load(t_game *g, const t_config *cfg)
{
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
	if (load_sprite_textures(g) != 0)
		return (error_msg("textures: failed to load sprite textures"));
	g->ceil_color = mlx_get_color_value(g->mlx, cfg->ceiling_color.value);
	g->floor_color = mlx_get_color_value(g->mlx, cfg->floor_color.value);
	return (0);
}

static int	load_wall_texture(t_game *g, const char *path, int index)
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

static void	put_pixel(t_img *tex, int x, int y, int color, int w, int h)
{
	int		bpp;
	char	*pp;

	bpp = tex->bpp / 8;
	if (x >= 0 && y >= 0 && x < w && y < h && bpp >= 4)
	{
		pp = tex->data + y * tex->line_len + x * bpp;
		pp[0] = (char)(color & 0xFF);
		pp[1] = (char)((color >> 8) & 0xFF);
		pp[2] = (char)((color >> 16) & 0xFF);
		pp[3] = 0;
	}
}

static int	generate_cyber_texture(t_game *g, int index)
{
	int		w;
	int		h;
	int		x;
	int		y;
	int		bpp;
	int		bg;
	int		grid;
	int		trace;
	int		color;
	char	*p;

	w = 64;
	h = 64;
	bg = 0x0A0A12;
	grid = 0x00FFCC;
	trace = 0x00FF66;

	if (index == TEX_NO)
	{
		grid = 0x00FF99;
		trace = 0x00FF33;
	}
	else if (index == TEX_SO)
	{
		grid = 0xFF3300;
		trace = 0xFFFF00;
	}
	else if (index == TEX_EA)
	{
		grid = 0x00AAFF;
		trace = 0x00FFFF;
	}
	else if (index == TEX_WE)
	{
		grid = 0xFF8800;
		trace = 0xFFAA00;
	}
	else if (index == TEX_DOOR)
	{
		grid = 0x00FF00;
		trace = 0x009900;
		bg = 0x111122;
	}
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
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			color = bg;
			if ((x % 8) == 0)
				color = grid;
			if ((y % 8) == 0)
				color = grid;
			if (((x + y) & 15) == 0)
				color = trace;
			if (((x - y) & 15) == 0)
				color = trace;
			if (index == TEX_DOOR)
			{
				if (x > 18 && x < 46 && y > 12 && y < 40)
				{
					if (y % 6 == 0 || x % 6 == 0)
						color = grid;
					else
						color = 0x001A10;
				}
				if ((x == 22 || x == 42) && y > 16 && y < 36)
					color = trace;
			}
			{
				p = g->tex[index].data + y * g->tex[index].line_len + x * bpp;
				p[0] = (char)(color & 0xFF);
				p[1] = (char)((color >> 8) & 0xFF);
				p[2] = (char)((color >> 16) & 0xFF);
				p[3] = 0;
			}
			x++;
		}
		y++;
	}
	return (0);
}

static int	load_sprite_textures(t_game *g)
{
	int		tex_index;
	int		sprite_id;
	int		frame;
	int		W;
	int		H;
	int		cx;
	int		body_w;
	int		body_h;
	int		shackle_y;
	int		is_open_phase;
	int		outline;
	int		y;
	int		x;
	int		bpp;
	char	*p;
	int		r_outer;
	int		thickness;
	int		theta;
	int		hx;
	int		hy;
	double	rot;
	int		slide_x;
	int		slide_y;
	double	rad;
	int		s;
	int		px0;
	int		py0;
	int		rx;
	int		ry;
	int		px;
	int		py;
	int		bx0;
	int		by0;
	int		bx1;
	int		by1;
	int		round_r;
	int		fill_top;
	int		fill_bot;
	int		t;
	int		r_t;
	int		g_t;
	int		b_t;
	int		r_b;
	int		g_b;
	int		b_b;
	int		rr;
	int		gg;
	int		bb;
	int		col;
	int		bezel_top;
	int		bezel_bot;
	int		cxm;
	int		cym;
	int		rim_outer;
	int		rim_inner;
	int		tick_color;
	int		rim_color;
	int		d2;
	int		deg;
	int		offset;
	double	ang;
	int		rx1;
	int		ry1;
	int		rx2;
	int		ry2;
	int		r2;
	int		offs;
	int		d;
	double	a;
	int		cxk;
	int		cyk;
	int		r;
	int		w;
	int		h2;
	int		sx;
	int		sy;

	tex_index = SPRITE_TEX_START;
	sprite_id = 0;
	frame = 0;

	while (sprite_id < 1 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		frame = 0;
		while (frame < 2 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
		{
			W = 64;
			H = 64;
			cx = W / 2;
			body_w = 28;
			body_h = 24;
			shackle_y = 26;
			is_open_phase = (frame == 1);
			if (is_open_phase)
				outline = 0x00D1FF;
			else
				outline = 0xFF3A6E;

			g->tex[tex_index].img = mlx_new_image(g->mlx, W, H);
			if (!g->tex[tex_index].img)
				return (error_msg("textures: sprite image creation failed"));
			g->tex[tex_index].data = mlx_get_data_addr(g->tex[tex_index].img,
					&g->tex[tex_index].bpp, &g->tex[tex_index].line_len, &g->tex[tex_index].endian);
			if (!g->tex[tex_index].data)
			{
				mlx_destroy_image(g->mlx, g->tex[tex_index].img);
				return (error_msg("textures: sprite data addr failed"));
			}
			g->tex[tex_index].width = W;
			g->tex[tex_index].height = H;
			y = 0;
			while (y < H)
			{
				x = 0;
				while (x < W)
				{
					bpp = g->tex[tex_index].bpp / 8;
					if (bpp >= 4)
					{
						p = g->tex[tex_index].data + y * g->tex[tex_index].line_len + x * bpp;
						p[0] = 0;
						p[1] = 0;
						p[2] = 0;
						p[3] = 0;
					}
					x++;
				}
				y++;
			}
			{
				bpp = g->tex[tex_index].bpp / 8;
				{
					r_outer = 14;
					thickness = 3;
					hx = cx - (r_outer - 1);
					hy = shackle_y;
					if (is_open_phase)
						rot = (25.0 * 3.141592653589793 / 180.0);
					else
						rot = 0.0;
					if (is_open_phase)
						slide_x = 6;
					else
						slide_x = 0;
					if (is_open_phase)
						slide_y = -4;
					else
						slide_y = 0;
					theta = -210;
					while (theta <= 10)
					{
						rad = theta * 3.141592653589793 / 180.0;
						s = 0;
						while (s < thickness)
						{
							px0 = cx + (int)(cos(rad) * (r_outer - s));
							py0 = shackle_y - 4 + (int)(sin(rad) * (r_outer - s));
							if (is_open_phase)
							{
								rx = px0 - hx;
								ry = py0 - hy;
								px = hx + (int)(rx * cos(rot) - ry * sin(rot)) + slide_x;
								py = hy + (int)(rx * sin(rot) + ry * cos(rot)) + slide_y;
								put_pixel(&g->tex[tex_index], px, py, outline, W, H);
							}
							else
							{
								put_pixel(&g->tex[tex_index], px0, py0, outline, W, H);
							}
							s++;
						}
						theta++;
					}
					y = shackle_y;
					while (y <= 34)
					{
						put_pixel(&g->tex[tex_index], hx, y, outline, W, H);
						y++;
					}
					y = shackle_y;
					while (y <= 34)
					{
						put_pixel(&g->tex[tex_index], hx + 1, y, (outline & 0xFEFEFE) >> 1, W, H);
						y++;
					}
					if (!is_open_phase)
					{
						y = shackle_y;
						while (y <= 34)
						{
							put_pixel(&g->tex[tex_index], cx + (r_outer - 1), y, outline, W, H);
							y++;
						}
						y = shackle_y;
						while (y <= 34)
						{
							put_pixel(&g->tex[tex_index], cx + (r_outer - 1) + 1, y, (outline & 0xFEFEFE) >> 1, W, H);
							y++;
						}
					}
				}
				{
					bx0 = cx - body_w / 2;
					by0 = 34;
					bx1 = cx + body_w / 2;
					by1 = by0 + body_h;
					round_r = 5;
					if (is_open_phase)
						fill_top = 0x0B2B33;
					else
						fill_top = 0x330B1B;
					if (is_open_phase)
						fill_bot = 0x071920;
					else
						fill_bot = 0x1A0710;
					y = by0;
					while (y <= by1)
					{
						if (body_h)
							t = (y - by0) * 255 / body_h;
						else
							t = (y - by0) * 255 / 1;
						r_t = (fill_top >> 16) & 0xFF;
						g_t = (fill_top >> 8) & 0xFF;
						b_t = fill_top & 0xFF;
						r_b = (fill_bot >> 16) & 0xFF;
						g_b = (fill_bot >> 8) & 0xFF;
						b_b = fill_bot & 0xFF;
						rr = (r_t * (255 - t) + r_b * t) / 255;
						gg = (g_t * (255 - t) + g_b * t) / 255;
						bb = (b_t * (255 - t) + b_b * t) / 255;
						col = (rr << 16) | (gg << 8) | bb;
						x = bx0;
						while (x <= bx1)
						{
							if (x < bx0 + round_r)
								rx = bx0 + round_r - x;
							else if (x > bx1 - round_r)
								rx = x - (bx1 - round_r);
							else
								rx = 0;
							if (y < by0 + round_r)
								ry = by0 + round_r - y;
							else if (y > by1 - round_r)
								ry = y - (by1 - round_r);
							else
								ry = 0;
							if (rx == 0)
							{
								if (ry == 0)
								{
									put_pixel(&g->tex[tex_index], x, y, col, W, H);
								}
							}
							else if (rx * rx + ry * ry <= round_r * round_r)
							{
								put_pixel(&g->tex[tex_index], x, y, col, W, H);
							}
							x++;
						}
						y++;
					}
					x = bx0;
					while (x <= bx1)
					{
						put_pixel(&g->tex[tex_index], x, by0, outline, W, H);
						put_pixel(&g->tex[tex_index], x, by1, outline, W, H);
						x++;
					}
					y = by0;
					while (y <= by1)
					{
						put_pixel(&g->tex[tex_index], bx0, y, outline, W, H);
						put_pixel(&g->tex[tex_index], bx1, y, outline, W, H);
						y++;
					}
					{
						if (is_open_phase)
							bezel_top = 0x19C3FF;
						else
							bezel_top = 0xFF6FA0;
						if (is_open_phase)
							bezel_bot = 0x0FA5E8;
						else
							bezel_bot = 0xE04C7B;
						x = bx0 + 1;
						while (x <= bx1 - 1)
						{
							put_pixel(&g->tex[tex_index], x, by0 + 1, bezel_top, W, H);
							put_pixel(&g->tex[tex_index], x, by1 - 1, bezel_bot, W, H);
							x++;
						}
						y = by0 + 1;
						while (y <= by1 - 1)
						{
							put_pixel(&g->tex[tex_index], bx0 + 1, y, bezel_top, W, H);
							put_pixel(&g->tex[tex_index], bx1 - 1, y, bezel_bot, W, H);
							y++;
						}
					}
					y = by0 + 2;
					while (y <= by1)
					{
						x = bx0 + 2;
						while (x <= bx1)
						{
							if (x == bx1)
							{
								put_pixel(&g->tex[tex_index], x, y, 0x080808, W, H);
							}
							if (y == by1)
							{
								put_pixel(&g->tex[tex_index], x, y, 0x080808, W, H);
							}
							x++;
						}
						y++;
					}
					y = by0 + 2;
					while (y <= by1 - 2)
					{
						if (((y - by0) % 7) == 0)
						{
							put_pixel(&g->tex[tex_index], bx0 + 3, y, (outline & 0xFEFEFE) >> 1, W, H);
						}
						y++;
					}
					{
						cxm = cx;
						cym = by0 + body_h / 2;
						rim_outer = 11;
						rim_inner = 7;
						if (is_open_phase)
							tick_color = 0x19C3FF;
						else
							tick_color = 0xFF6FA0;
						if (is_open_phase)
							rim_color = 0x0FA5E8;
						else
							rim_color = 0xD44B78;
						y = -rim_outer;
						while (y <= rim_outer)
						{
							x = -rim_outer;
							while (x <= rim_outer)
							{
								d2 = x * x + y * y;
								if (d2 <= rim_outer * rim_outer)
								{
									if (d2 >= (rim_outer - 1) * (rim_outer - 1))
									{
										put_pixel(&g->tex[tex_index], cxm + x, cym + y, rim_color, W, H);
									}
								}
								if (d2 <= rim_inner * rim_inner)
								{
									if (d2 >= (rim_inner - 1) * (rim_inner - 1))
									{
										put_pixel(&g->tex[tex_index], cxm + x, cym + y, rim_color, W, H);
									}
								}
								x++;
							}
							y++;
						}
						{
							if (is_open_phase)
								offset = 10;
							else
								offset = 0;
							deg = 0;
							while (deg < 360)
							{
								ang = (deg + offset) * 3.141592653589793 / 180.0;
								rx1 = (int)(cos(ang) * (rim_inner - 1));
								ry1 = (int)(sin(ang) * (rim_inner - 1));
								rx2 = (int)(cos(ang) * (rim_outer + 1));
								ry2 = (int)(sin(ang) * (rim_outer + 1));
								t = 0;
								while (t < 3)
								{
									px = cxm + rx1 + (rx2 - rx1) * t / 3;
									py = cym + ry1 + (ry2 - ry1) * t / 3;
									put_pixel(&g->tex[tex_index], px, py, tick_color, W, H);
									t++;
								}
								deg += 20;
							}
						}
						{
							r2 = 4;
							if (is_open_phase)
								offs = 6;
							else
								offs = 0;
							d = 0;
							while (d < 360)
							{
								a = (d - offs) * 3.141592653589793 / 180.0;
								px = cxm + (int)(cos(a) * r2);
								py = cym + (int)(sin(a) * r2);
								put_pixel(&g->tex[tex_index], px, py, tick_color, W, H);
								d += 60;
							}
						}
						put_pixel(&g->tex[tex_index], cxm, cym, tick_color, W, H);
					}
					{
						cxk = cx;
						cyk = by0 + body_h / 2 + 2;
						if (!is_open_phase)
						{
							r = 4;
							y = -r;
							while (y <= r)
							{
								x = -r;
								while (x <= r)
								{
									if (x * x + y * y <= r * r)
									{
										put_pixel(&g->tex[tex_index], cxk + x, cyk + y, 0x111111, W, H);
									}
									x++;
								}
								y++;
							}
							y = 0;
							while (y < 7)
							{
								put_pixel(&g->tex[tex_index], cxk, cyk + r + y, 0x111111, W, H);
								y++;
							}
						}
						else
						{
							w = 10;
							h2 = 10;
							x = -w / 2;
							while (x <= w / 2)
							{
								put_pixel(&g->tex[tex_index], cxk + x, cyk - h2 / 2, 0x0B9ED6, W, H);
								put_pixel(&g->tex[tex_index], cxk + x, cyk + h2 / 2, 0x0B9ED6, W, H);
								x++;
							}
							y = -h2 / 2;
							while (y <= h2 / 2)
							{
								put_pixel(&g->tex[tex_index], cxk - w / 2, cyk + y, 0x0B9ED6, W, H);
								put_pixel(&g->tex[tex_index], cxk + w / 2, cyk + y, 0x0B9ED6, W, H);
								y++;
							}
							x = -w / 2 + 1;
							while (x <= w / 2 - 1)
							{
								if (x % 2 == 0)
								{
									offset = 1;
								}
								else
								{
									offset = 0;
								}
								put_pixel(&g->tex[tex_index], cxk + x, cyk + h2 / 2 + offset, 0x0B9ED6, W, H);
								x++;
							}
						}
					}
					if (is_open_phase)
					{
						sx = (cx - (14 - 1)) + 2;
						sy = shackle_y + 1;
						put_pixel(&g->tex[tex_index], sx, sy, 0x19C3FF, W, H);
						put_pixel(&g->tex[tex_index], sx + 1, sy, 0x0FA5E8, W, H);
						put_pixel(&g->tex[tex_index], sx, sy + 1, 0x0FA5E8, W, H);
					}
				}
			}
			tex_index++;
			frame++;
		}
		sprite_id++;
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
