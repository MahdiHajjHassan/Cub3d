#include "cub3d_bonus.h"
#include "mlx.h"

static int iabs(int v) { return (v < 0) ? -v : v; }
static void sort_sprites_by_distance(t_game *g);
static void render_single_sprite(t_game *g, t_sprite *sprite);
static void draw_sprite_column(t_game *g, t_sprite *sprite, int stripe, int draw_start_y, int draw_end_y, int sprite_height, int sprite_screen_x, int sprite_width);

double get_time_delta(struct timeval *last_time)
{
	struct timeval current_time;
	double delta;
	
	gettimeofday(&current_time, NULL);
	delta = (current_time.tv_sec - last_time->tv_sec) + 
			(current_time.tv_usec - last_time->tv_usec) / 1000000.0;
	*last_time = current_time;
	return (delta);
}

void update_sprites(t_game *g)
{
	int i;
	double frame_duration = 0.2; /* 200ms per frame */
	
	i = 0;
	while (i < g->map->sprite_count)
	{
		t_sprite *sprite = &g->map->sprites[i];
		
		/* Calculate distance to player for depth sorting */
		double dx = sprite->x - g->pos_x;
		double dy = sprite->y - g->pos_y;
		sprite->distance = dx * dx + dy * dy; /* Squared distance is sufficient for sorting */
		
		/* Update animation if sprite is animated */
		if (sprite->animated && sprite->frame_count > 1)
		{
			sprite->frame_timer += g->frame_time;
			if (sprite->frame_timer >= frame_duration)
			{
				sprite->current_frame = (sprite->current_frame + 1) % sprite->frame_count;
				sprite->frame_timer = 0.0;
			}
		}
		i++;
	}
	
	/* Sort sprites by distance (farthest first for proper rendering) */
	sort_sprites_by_distance(g);
}

static void sort_sprites_by_distance(t_game *g)
{
	int i, j;
	t_sprite temp;
	
	/* Simple bubble sort - adequate for small number of sprites */
	i = 0;
	while (i < g->map->sprite_count - 1)
	{
		j = 0;
		while (j < g->map->sprite_count - i - 1)
		{
			if (g->map->sprites[j].distance < g->map->sprites[j + 1].distance)
			{
				temp = g->map->sprites[j];
				g->map->sprites[j] = g->map->sprites[j + 1];
				g->map->sprites[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void render_sprites(t_game *g)
{
	int i;
	
	i = 0;
	while (i < g->map->sprite_count)
	{
		render_single_sprite(g, &g->map->sprites[i]);
		i++;
	}
}

static void render_single_sprite(t_game *g, t_sprite *sprite)
{
	/* Transform sprite with the inverse camera matrix */
	double sprite_x = sprite->x - g->pos_x;
	double sprite_y = sprite->y - g->pos_y;
	
	/* Required for correct matrix multiplication */
	double inv_det = 1.0 / (g->plane_x * g->dir_y - g->dir_x * g->plane_y);
	
	double transform_x = inv_det * (g->dir_y * sprite_x - g->dir_x * sprite_y);
	double transform_y = inv_det * (-g->plane_y * sprite_x + g->plane_x * sprite_y);
	
	/* Don't render sprites behind the player */
	if (transform_y <= 0)
		return;
	
	int sprite_screen_x = (int)((WIN_W / 2) * (1 + transform_x / transform_y));
	
	/* Calculate height of the sprite on screen */
	int sprite_height = iabs((int)(WIN_H / transform_y));
	
	/* Calculate lowest and highest pixel to fill in current stripe */
	int draw_start_y = -sprite_height / 2 + WIN_H / 2;
	if (draw_start_y < 0)
		draw_start_y = 0;
	int draw_end_y = sprite_height / 2 + WIN_H / 2;
	if (draw_end_y >= WIN_H)
		draw_end_y = WIN_H - 1;
	
	/* Calculate width of the sprite */
	int sprite_width = iabs((int)(WIN_H / transform_y));
	int draw_start_x = -sprite_width / 2 + sprite_screen_x;
	if (draw_start_x < 0)
		draw_start_x = 0;
	int draw_end_x = sprite_width / 2 + sprite_screen_x;
	if (draw_end_x >= WIN_W)
		draw_end_x = WIN_W - 1;
	
	/* Loop through every vertical stripe of the sprite on screen */
	int stripe = draw_start_x;
	while (stripe < draw_end_x)
	{
		draw_sprite_column(g, sprite, stripe, draw_start_y, draw_end_y, 
						  sprite_height, sprite_screen_x, sprite_width);
		stripe++;
	}
}

static void draw_sprite_column(t_game *g, t_sprite *sprite, int stripe, int draw_start_y, int draw_end_y, int sprite_height, int sprite_screen_x, int sprite_width)
{
	int tex_id = SPRITE_TEX_START + sprite->texture_id * MAX_SPRITE_FRAMES + sprite->current_frame;
	t_img *tex = &g->tex[tex_id];
	
	if (!tex->data || tex_id >= TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
		return;
	
	int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * tex->width / sprite_width) / 256;
	
	/* Make sure tex_x is within bounds */
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	
	int y = draw_start_y;
	while (y < draw_end_y)
	{
		int d = (y) * 256 - WIN_H * 128 + sprite_height * 128;
		int tex_y = ((d * tex->height) / sprite_height) / 256;
		
		/* Make sure tex_y is within bounds */
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		
		/* Get texture pixel */
		int tex_bpp = tex->bpp / 8;
		int frame_bpp = g->frame.bpp / 8;
		
		if (tex_bpp >= 4 && frame_bpp >= 4)
		{
			char *tex_pixel = tex->data + tex_y * tex->line_len + tex_x * tex_bpp;
			char *frame_pixel = g->frame.data + y * g->frame.line_len + stripe * frame_bpp;
			
			/* Check for transparency (assuming black is transparent) */
			if (tex_pixel[0] != 0 || tex_pixel[1] != 0 || tex_pixel[2] != 0)
			{
				frame_pixel[0] = tex_pixel[0];
				frame_pixel[1] = tex_pixel[1];
				frame_pixel[2] = tex_pixel[2];
				frame_pixel[3] = 0;
			}
		}
		y++;
	}
}
