/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:45:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/10 16:45:06 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
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
	
	/* Create one animated cybersecurity lock (2 frames: closed/open) */
	while (sprite_id < 1 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		frame = 0;
		while (frame < 2 && tex_index < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
		{
			int W = 64, H = 64;
			int cx = W / 2;
			int body_w = 28, body_h = 24;
			int shackle_y = 26;
			/* Palette: frame 1 -> open (cyan), frame 0 -> locked (magenta/red) */
			int is_open_phase = (frame == 1);
			int outline = is_open_phase ? 0x00D1FF : 0xFF3A6E; /* shackle/body outline */
			int y;
			int x;
			
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

			/* Clear to transparent (renderer treats black as transparent) */
			y = 0;
			while (y < H)
			{
				x = 0;
				while (x < W)
				{
					int bpp = g->tex[tex_index].bpp / 8;
					if (bpp >= 4)
					{
						char *p = g->tex[tex_index].data + y * g->tex[tex_index].line_len + x * bpp;
						p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0;
					}
					x++;
				}
				y++;
			}

			/* Convenience macro for pixel write with bounds */
			{
				int bpp = g->tex[tex_index].bpp / 8;
				#define PUTPX(XX,YY,COL) do { \
					int _x=(XX), _y=(YY); \
					if (_x>=0 && _y>=0 && _x<W && _y<H && bpp>=4) { \
						char *pp = g->tex[tex_index].data + _y * g->tex[tex_index].line_len + _x * bpp; \
						pp[0]=(char)((COL)&0xFF); \
						pp[1]=(char)(((COL)>>8)&0xFF); \
						pp[2]=(char)(((COL)>>16)&0xFF); \
						pp[3]=0; \
					} \
				} while(0)

				/* No halo background: keep transparency around the lock */

				/* Shackle: pivot-and-slide opening around left hinge */
				{
					int r_outer = 14;
					int thickness = 3;
					int theta;
					/* hinge (left leg top) */
					int hx = cx - (r_outer - 1);
					int hy = shackle_y;
					double rot = is_open_phase ? (25.0 * 3.141592653589793 / 180.0) : 0.0; /* rotate ~25° when open */
					int slide_x = is_open_phase ? 6 : 0;
					int slide_y = is_open_phase ? -4 : 0;
					for (theta = -210; theta <= 10; ++theta)
					{
						double rad = theta * 3.141592653589793 / 180.0;
						int s;
						for (s = 0; s < thickness; ++s)
						{
							int px0 = cx + (int)(cos(rad) * (r_outer - s));
							int py0 = shackle_y - 4 + (int)(sin(rad) * (r_outer - s));
							if (is_open_phase)
							{
								/* rotate around hinge and slide */
								int rx = px0 - hx;
								int ry = py0 - hy;
								int px = hx + (int)(rx * cos(rot) - ry * sin(rot)) + slide_x;
								int py = hy + (int)(rx * sin(rot) + ry * cos(rot)) + slide_y;
								PUTPX(px, py, outline);
							}
							else
							{
								PUTPX(px0, py0, outline);
							}
						}
					}
					/* legs: open -> only left leg remains; closed -> both legs */
					for (y = shackle_y; y <= 34; ++y)
						PUTPX(hx, y, outline);
					/* subtle outline glow for left leg */
					for (y = shackle_y; y <= 34; ++y)
						PUTPX(hx + 1, y, (outline & 0xFEFEFE) >> 1);
					if (!is_open_phase)
					{
						for (y = shackle_y; y <= 34; ++y)
							PUTPX(cx + (r_outer - 1), y, outline);
						/* subtle outline glow for right leg when closed */
						for (y = shackle_y; y <= 34; ++y)
							PUTPX(cx + (r_outer - 1) + 1, y, (outline & 0xFEFEFE) >> 1);
					}
				}

				/* Lock body: rounded rectangle with vertical gradient and inner bezel */
				{
					int bx0 = cx - body_w / 2;
					int by0 = 34;
					int bx1 = cx + body_w / 2;
					int by1 = by0 + body_h;
					int round_r = 5;
					int fill_top = is_open_phase ? 0x0B2B33 : 0x330B1B; /* top gradient */
					int fill_bot = is_open_phase ? 0x071920 : 0x1A0710; /* bottom gradient */
					for (y = by0; y <= by1; ++y)
					{
						int t = (y - by0) * 255 / (body_h ? body_h : 1);
						int r_t = (fill_top >> 16) & 0xFF, g_t = (fill_top >> 8) & 0xFF, b_t = fill_top & 0xFF;
						int r_b = (fill_bot >> 16) & 0xFF, g_b = (fill_bot >> 8) & 0xFF, b_b = fill_bot & 0xFF;
						int rr = (r_t * (255 - t) + r_b * t) / 255;
						int gg = (g_t * (255 - t) + g_b * t) / 255;
						int bb = (b_t * (255 - t) + b_b * t) / 255;
						int col = (rr << 16) | (gg << 8) | bb;
						for (x = bx0; x <= bx1; ++x)
						{
							int rx = (x < bx0 + round_r) ? (bx0 + round_r - x) : (x > bx1 - round_r ? x - (bx1 - round_r) : 0);
							int ry = (y < by0 + round_r) ? (by0 + round_r - y) : (y > by1 - round_r ? y - (by1 - round_r) : 0);
							if (rx == 0 && ry == 0)
								PUTPX(x, y, col);
							else if (rx*rx + ry*ry <= round_r*round_r)
								PUTPX(x, y, col);
						}
					}
					/* outer outline */
					for (x = bx0; x <= bx1; ++x) { PUTPX(x, by0, outline); PUTPX(x, by1, outline); }
					for (y = by0; y <= by1; ++y) { PUTPX(bx0, y, outline); PUTPX(bx1, y, outline); }
					/* inner bezel */
					{
						int bezel_top = is_open_phase ? 0x19C3FF : 0xFF6FA0;
						int bezel_bot = is_open_phase ? 0x0FA5E8 : 0xE04C7B;
						for (x = bx0 + 1; x <= bx1 - 1; ++x) { PUTPX(x, by0 + 1, bezel_top); PUTPX(x, by1 - 1, bezel_bot); }
						for (y = by0 + 1; y <= by1 - 1; ++y) { PUTPX(bx0 + 1, y, bezel_top); PUTPX(bx1 - 1, y, bezel_bot); }
					}
					/* soft drop shadow (bottom-right) */
					for (y = by0 + 2; y <= by1; ++y)
						for (x = bx0 + 2; x <= bx1; ++x)
							if (x == bx1 || y == by1) PUTPX(x, y, 0x080808);
					/* specular highlight strip on left */
					for (y = by0 + 2; y <= by1 - 2; ++y)
						if (((y - by0) % 7) == 0) PUTPX(bx0 + 3, y, (outline & 0xFEFEFE) >> 1);

					/* Rotating cipher wheel motif (concentric rings + tick marks) */
					{
						int cxm = cx;
						int cym = by0 + body_h/2;
						int rim_outer = 11;
						int rim_inner = 7;
						int tick_color = is_open_phase ? 0x19C3FF : 0xFF6FA0;
						int rim_color  = is_open_phase ? 0x0FA5E8 : 0xD44B78;
						/* draw rims */
						for (y = -rim_outer; y <= rim_outer; ++y)
						{
							for (x = -rim_outer; x <= rim_outer; ++x)
							{
								int d2 = x*x + y*y;
								if (d2 <= rim_outer*rim_outer && d2 >= (rim_outer-1)*(rim_outer-1)) PUTPX(cxm + x, cym + y, rim_color);
								if (d2 <= rim_inner*rim_inner && d2 >= (rim_inner-1)*(rim_inner-1)) PUTPX(cxm + x, cym + y, rim_color);
							}
						}
						/* tick marks around the ring; offset per frame for rotation */
						{
							int deg;
							int offset = is_open_phase ? 10 : 0; /* degrees */
							for (deg = 0; deg < 360; deg += 20)
							{
								double ang = (deg + offset) * 3.141592653589793 / 180.0;
								int rx1 = (int)(cos(ang) * (rim_inner - 1));
								int ry1 = (int)(sin(ang) * (rim_inner - 1));
								int rx2 = (int)(cos(ang) * (rim_outer + 1));
								int ry2 = (int)(sin(ang) * (rim_outer + 1));
								/* draw small tick from inner to outer */
								int t;
								for (t = 0; t < 3; ++t)
								{
									int px = cxm + rx1 + (rx2 - rx1) * t / 3;
									int py = cym + ry1 + (ry2 - ry1) * t / 3;
									PUTPX(px, py, tick_color);
								}
							}
						}
						/* inner counter-rotating micro ring */
						{
							int r2 = 4;
							int offs = is_open_phase ? 6 : 0;
							int d;
							for (d = 0; d < 360; d += 60)
							{
								double a = (d - offs) * 3.141592653589793 / 180.0;
								int px = cxm + (int)(cos(a) * r2);
								int py = cym + (int)(sin(a) * r2);
								PUTPX(px, py, tick_color);
							}
						}
						/* small inner dot for depth */
						PUTPX(cxm, cym, tick_color);
					}

					/* Keyhole: changes to a shield glyph when open */
					{
						int cxk = cx;
						int cyk = by0 + body_h/2 + 2;
						if (!is_open_phase)
						{
							/* classic keyhole */
							int r = 4;
							for (y = -r; y <= r; ++y)
								for (x = -r; x <= r; ++x)
									if (x*x + y*y <= r*r) PUTPX(cxk + x, cyk + y, 0x111111);
							for (y = 0; y < 7; ++y) PUTPX(cxk, cyk + r + y, 0x111111);
						}
						else
						{
							/* minimal shield */
							int w = 10, h2 = 10;
							for (x = -w/2; x <= w/2; ++x) { PUTPX(cxk + x, cyk - h2/2, 0x0B9ED6); PUTPX(cxk + x, cyk + h2/2, 0x0B9ED6); }
							for (y = -h2/2; y <= h2/2; ++y) { PUTPX(cxk - w/2, cyk + y, 0x0B9ED6); PUTPX(cxk + w/2, cyk + y, 0x0B9ED6); }
							/* chevron bottom */
							for (x = -w/2 + 1; x <= w/2 - 1; ++x) PUTPX(cxk + x, cyk + h2/2 + (x % 2 == 0 ? 1 : 0), 0x0B9ED6);
						}
					}

					/* hinge spark when opening */
					if (is_open_phase)
					{
						int sx = (cx - (14 - 1)) + 2; /* near left hinge */
						int sy = shackle_y + 1;
						PUTPX(sx, sy, 0x19C3FF);
						PUTPX(sx+1, sy, 0x0FA5E8);
						PUTPX(sx, sy+1, 0x0FA5E8);
					}
				}
 
				#undef PUTPX
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
