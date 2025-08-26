#include "cub3d_bonus.h"
#include "mlx.h"

static int iabs(int v) { return (v < 0) ? -v : v; }
static void draw_minimap_pixel(t_img *img, int x, int y, int color);
static void draw_minimap_rect(t_img *img, int x, int y, int size, int color);
static void draw_player_on_minimap(t_game *g);

int init_minimap(t_game *g)
{
	int max_dim;
	int fit_scale;

	g->minimap.size = MINIMAP_SIZE;
	g->minimap.margin = MINIMAP_MARGIN;

	/* Choose a scale that fits the entire map within the minimap image. */
	max_dim = (int)((g->map->width > g->map->height) ? g->map->width : g->map->height);
	if (max_dim <= 0)
		fit_scale = MINIMAP_SCALE;
	else
		fit_scale = (g->minimap.size - 2) / max_dim; /* leave 1px border */
	if (fit_scale <= 0)
		fit_scale = 1;
	g->minimap.scale = fit_scale;

	/* Compute offsets so the map is centered inside the minimap image. */
	g->minimap.offset_x = (g->minimap.size - (int)g->map->width * g->minimap.scale) / 2;
	g->minimap.offset_y = (g->minimap.size - (int)g->map->height * g->minimap.scale) / 2;
	if (g->minimap.offset_x < 0) g->minimap.offset_x = 0;
	if (g->minimap.offset_y < 0) g->minimap.offset_y = 0;

	g->minimap.img.img = mlx_new_image(g->mlx, g->minimap.size, g->minimap.size);
	if (!g->minimap.img.img)
		return (error_msg("minimap: image creation failed"));

	g->minimap.img.data = mlx_get_data_addr(g->minimap.img.img,
		&g->minimap.img.bpp, &g->minimap.img.line_len, &g->minimap.img.endian);
	if (!g->minimap.img.data)
	{
		mlx_destroy_image(g->mlx, g->minimap.img.img);
		return (error_msg("minimap: data addr failed"));
	}

	g->minimap.img.width = g->minimap.size;
	g->minimap.img.height = g->minimap.size;

	return (0);
}

void destroy_minimap(t_game *g)
{
	if (g->mlx && g->minimap.img.img)
		mlx_destroy_image(g->mlx, g->minimap.img.img);
	g->minimap.img = (t_img){0};
}

static void draw_minimap_pixel(t_img *img, int x, int y, int color)
{
	int bpp;
	char *p;

	if (!img || !img->data || x < 0 || y < 0 || x >= img->width || y >= img->height)
		return;

	bpp = img->bpp / 8;
	if (bpp < 4)
		return;

	p = img->data + y * img->line_len + x * bpp;
	p[0] = (char)(color & 0xFF);
	p[1] = (char)((color >> 8) & 0xFF);
	p[2] = (char)((color >> 16) & 0xFF);
	p[3] = 0;
}

static void draw_minimap_rect(t_img *img, int x, int y, int size, int color)
{
	int i, j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			draw_minimap_pixel(img, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

static void draw_player_on_minimap(t_game *g)
{
	int map_x, map_y;
	int player_size = 3;
	int color = 0xFF0000; /* Red for player */

	/* Player position on minimap (apply center offset) */
	map_x = g->minimap.offset_x + (int)(g->pos_x * g->minimap.scale);
	map_y = g->minimap.offset_y + (int)(g->pos_y * g->minimap.scale);

	/* Center the rectangle around the player position */
	map_x -= player_size / 2;
	map_y -= player_size / 2;

	draw_minimap_rect(&g->minimap.img, map_x, map_y, player_size, color);

	/* Draw direction line */
	int dir_len = 10;
	int end_x = map_x + player_size / 2 + (int)(g->dir_x * dir_len);
	int end_y = map_y + player_size / 2 + (int)(g->dir_y * dir_len);

	/* Simple line drawing */
	int dx = end_x - (map_x + player_size / 2);
	int dy = end_y - (map_y + player_size / 2);
	int steps = (iabs(dx) > iabs(dy)) ? iabs(dx) : iabs(dy);

	if (steps > 0)
	{
		float x_inc = dx / (float)steps;
		float y_inc = dy / (float)steps;
		float x = map_x + player_size / 2;
		float y = map_y + player_size / 2;

		int i = 0;
		while (i <= steps)
		{
			draw_minimap_pixel(&g->minimap.img, (int)x, (int)y, color);
			x += x_inc;
			y += y_inc;
			i++;
		}
	}
}

void render_minimap(t_game *g)
{
	int x, y;
	int cell_size;

	if (!g || g->closing)
		return;
	cell_size = g->minimap.scale;
	/* Clear minimap with black background */
	x = 0;
	while (x < g->minimap.size)
	{
		y = 0;
		while (y < g->minimap.size)
		{
			draw_minimap_pixel(&g->minimap.img, x, y, 0x000000);
			y++;
		}
		x++;
	}

	/* Draw map cells (centered) */
	y = 0;
	while (y < (int)g->map->height)
	{
		x = 0;
		while (x < (int)g->map->width)
		{
			char cell = g->map->grid[y][x];
			int color = 0x000000; /* Black for empty */
			int map_x = g->minimap.offset_x + x * cell_size;
			int map_y = g->minimap.offset_y + y * cell_size;

			if (cell == '1')
				color = 0xFFFFFF; /* White for walls */
			else if (cell == 'D')
			{
				/* Check if door is open */
				int i = 0;
				bool is_open = false;
				while (i < g->map->door_count)
				{
					if (g->map->doors[i].x == x && g->map->doors[i].y == y)
					{
						is_open = g->map->doors[i].is_open;
						break;
					}
					i++;
				}
				color = is_open ? 0x00FF00 : 0x8B4513; /* Green if open, brown if closed */
			}
			else if (cell == '0' || cell == ' ')
				color = 0x404040; /* Dark gray for empty spaces */

			/* Draw cell on minimap */
			draw_minimap_rect(&g->minimap.img, map_x, map_y, cell_size, color);
			x++;
		}
		y++;
	}

	/* Draw sprites on minimap */
	int i = 0;
	while (i < g->map->sprite_count)
	{
		int sprite_x = g->minimap.offset_x + (int)(g->map->sprites[i].x * cell_size) - 1;
		int sprite_y = g->minimap.offset_y + (int)(g->map->sprites[i].y * cell_size) - 1;
		draw_minimap_rect(&g->minimap.img, sprite_x, sprite_y, 2, 0x0000FF); /* Blue for sprites */
		i++;
	}

	/* Draw player */
	draw_player_on_minimap(g);

	/* Put minimap to window */
	if (!g->closing && g->mlx && g->win)
		mlx_put_image_to_window(g->mlx, g->win, g->minimap.img.img,
			g->minimap.margin, g->minimap.margin);
}
