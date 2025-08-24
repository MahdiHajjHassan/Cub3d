#include "cub3d_bonus.h"
#include "mlx.h"

/* Forward declarations */
int frame_init(t_game *g);
void frame_destroy(t_game *g);
static void fill_rect(t_img *img, int x0, int y0, int x1, int y1, int color);
static void draw_vertical_stripe(t_game *g, int x, int tex_id, int draw_start, int draw_end, int line_height, double wall_x);
void render_frame_bonus(t_game *g);

/* Helper to clear a rectangle with a solid color (BGRA write) */
static void fill_rect(t_img *img, int x0, int y0, int x1, int y1, int color)
{
	int y;
	int x;
	int bpp;
	char *p;

	if (!img || !img->data)
		return;
	bpp = img->bpp / 8;
	if (bpp < 4)
		return;
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x1 > img->width) x1 = img->width;
	if (y1 > img->height) y1 = img->height;
	y = y0;
	while (y < y1)
	{
		x = x0;
		while (x < x1)
		{
			p = img->data + y * img->line_len + x * bpp;
			p[0] = (char)(color & 0xFF);
			p[1] = (char)((color >> 8) & 0xFF);
			p[2] = (char)((color >> 16) & 0xFF);
			p[3] = 0;
			x++;
		}
		y++;
	}
}

/* Initialize frame buffer image */
int frame_init(t_game *g)
{
	g->frame.img = mlx_new_image(g->mlx, WIN_W, WIN_H);
	if (!g->frame.img)
		return (error_msg("mlx: image failed"));
	g->frame.data = mlx_get_data_addr(g->frame.img, &g->frame.bpp, &g->frame.line_len, &g->frame.endian);
	if (!g->frame.data)
		return (mlx_destroy_image(g->mlx, g->frame.img), g->frame.img=NULL, error_msg("mlx: data addr failed"));
	g->frame.width = WIN_W;
	g->frame.height = WIN_H;
	return (0);
}

void frame_destroy(t_game *g)
{
	if (g->mlx && g->frame.img)
		mlx_destroy_image(g->mlx, g->frame.img);
	g->frame = (t_img){0};
}

/* Enhanced DDA raycasting core to render walls with doors */
static void draw_vertical_stripe(t_game *g, int x, int tex_id, int draw_start, int draw_end, int line_height, double wall_x)
{
	/* Select texture and derive stepping */
	t_img *tx = &g->tex[tex_id];
	int tex_x;
	double step;
	double tex_pos;
	int y;
	int bpp = g->frame.bpp / 8;
	int tbpp = tx->bpp / 8;

	if (tbpp < 4 || bpp < 4)
		return;
	tex_x = (int)(wall_x * (double)tx->width);
	if (tex_x < 0) tex_x = 0;
	if (tex_x >= tx->width) tex_x = tx->width - 1;
	/* How much to increase the texture coordinate per screen pixel */
	step = 1.0 * tx->height / line_height;
	/* Starting texture coordinate */
	tex_pos = (draw_start - WIN_H / 2 + line_height / 2) * step;
	y = draw_start;
	while (y < draw_end)
	{
		int tex_y = (int)tex_pos % (tx->height ? tx->height : 1);
		tex_pos += step;
		char *src = tx->data + tex_y * tx->line_len + tex_x * tbpp;
		char *dst = g->frame.data + y * g->frame.line_len + x * bpp;
		/* copy BGRA */
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = 0;
		y++;
	}
}

void render_frame_bonus(t_game *g)
{
	int x;

	/* Draw ceiling and floor */
	fill_rect(&g->frame, 0, 0, WIN_W, WIN_H/2, g->ceil_color);
	fill_rect(&g->frame, 0, WIN_H/2, WIN_W, WIN_H, g->floor_color);

	x = 0;
	while (x < WIN_W)
	{
		double camera_x = 2.0 * x / (double)WIN_W - 1.0;
		double ray_dir_x = g->dir_x + g->plane_x * camera_x;
		double ray_dir_y = g->dir_y + g->plane_y * camera_x;

		int map_x = (int)g->pos_x;
		int map_y = (int)g->pos_y;

		double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1.0 / ray_dir_x);
		double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1.0 / ray_dir_y);
		double side_dist_x;
		double side_dist_y;
		int step_x;
		int step_y;
		int side;
		bool hit_door = false;

		if (ray_dir_x < 0) { step_x = -1; side_dist_x = (g->pos_x - map_x) * delta_dist_x; }
		else { step_x = 1; side_dist_x = (map_x + 1.0 - g->pos_x) * delta_dist_x; }
		if (ray_dir_y < 0) { step_y = -1; side_dist_y = (g->pos_y - map_y) * delta_dist_y; }
		else { step_y = 1; side_dist_y = (map_y + 1.0 - g->pos_y) * delta_dist_y; }

		/* DDA */
		while (1)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (map_x < 0 || map_y < 0 || map_x >= (int)g->map->width || map_y >= (int)g->map->height)
				break;
			
			char cell = g->map->grid[map_y][map_x];
			if (cell == '1')
				break;
			else if (cell == 'D')
			{
				/* Check if door is closed */
				int i = 0;
				bool door_is_open = false;
				while (i < g->map->door_count)
				{
					if (g->map->doors[i].x == map_x && g->map->doors[i].y == map_y)
					{
						door_is_open = g->map->doors[i].is_open;
						break;
					}
					i++;
				}
				if (!door_is_open)
				{
					hit_door = true;
					break;
				}
			}
		}

		/* Perp distance */
		double perp_wall_dist;
		if (side == 0)
			perp_wall_dist = (map_x - g->pos_x + (1 - step_x) / 2.0) / (ray_dir_x != 0 ? ray_dir_x : 1e-6);
		else
			perp_wall_dist = (map_y - g->pos_y + (1 - step_y) / 2.0) / (ray_dir_y != 0 ? ray_dir_y : 1e-6);
		if (perp_wall_dist < 1e-6) perp_wall_dist = 1e-6;

		int line_height = (int)(WIN_H / perp_wall_dist);
		int draw_start = -line_height / 2 + WIN_H / 2;
		int draw_end = line_height / 2 + WIN_H / 2;
		if (draw_start < 0) draw_start = 0;
		if (draw_end > WIN_H) draw_end = WIN_H;

		/* Choose texture and compute wall_x for tex X coordinate */
		int tex_id;
		double wall_x;
		if (side == 0)
			wall_x = g->pos_y + perp_wall_dist * ray_dir_y;
		else
			wall_x = g->pos_x + perp_wall_dist * ray_dir_x;
		wall_x -= floor(wall_x);

		/* Select appropriate texture */
		if (hit_door)
			tex_id = TEX_DOOR;
		else if (side == 0 && ray_dir_x > 0) tex_id = TEX_WE; /* west wall seen from east -> use WE */
		else if (side == 0 && ray_dir_x < 0) tex_id = TEX_EA;
		else if (side == 1 && ray_dir_y > 0) tex_id = TEX_NO;
		else tex_id = TEX_SO;

		draw_vertical_stripe(g, x, tex_id, draw_start, draw_end, line_height, wall_x);
		x++;
	}

	/* Render sprites after walls (for proper depth) */
	render_sprites(g);

	/* Put the frame first */
	mlx_put_image_to_window(g->mlx, g->win, g->frame.img, 0, 0);

	/* Render minimap on top (drawn after frame) */
	render_minimap(g);
}

