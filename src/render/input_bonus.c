#include "cub3d_bonus.h"
#include "mlx.h"

static int iabs(int v) { return (v < 0) ? -v : v; }
static bool check_collision(t_game *g, double new_x, double new_y);
static bool is_door_at_position(t_game *g, int x, int y);

/* Enhanced collision detection with wall collision prevention */
static bool check_collision(t_game *g, double new_x, double new_y)
{
	int map_x = (int)new_x;
	int map_y = (int)new_y;
	
	/* Check boundaries */
	if (map_x < 0 || map_y < 0 || map_x >= (int)g->map->width || map_y >= (int)g->map->height)
		return (true);
	
	/* Check if position is a wall */
	char cell = g->map->grid[map_y][map_x];
	if (cell == '1')
		return (true);
	
	/* Check if position is a closed door */
	if (cell == 'D')
	{
		int i = 0;
		while (i < g->map->door_count)
		{
			if (g->map->doors[i].x == map_x && g->map->doors[i].y == map_y)
			{
				if (!g->map->doors[i].is_open)
					return (true);
				break;
			}
			i++;
		}
	}
	
	return (false);
}

static bool is_door_at_position(t_game *g, int x, int y)
{
	if (x < 0 || y < 0 || x >= (int)g->map->width || y >= (int)g->map->height)
		return (false);
	
	return (g->map->grid[y][x] == 'D');
}

void handle_door_interaction(t_game *g)
{
	/* Check the cell directly in front of the player */
	double check_dist = 1.5; /* Distance to check for doors */
	double check_x = g->pos_x + g->dir_x * check_dist;
	double check_y = g->pos_y + g->dir_y * check_dist;
	
	int door_x = (int)check_x;
	int door_y = (int)check_y;
	
	if (is_door_at_position(g, door_x, door_y))
	{
		/* Find the door and toggle its state */
		int i = 0;
		while (i < g->map->door_count)
		{
			if (g->map->doors[i].x == door_x && g->map->doors[i].y == door_y)
			{
				g->map->doors[i].is_open = !g->map->doors[i].is_open;
				g->map->doors[i].open_timer = 0.0;
				break;
			}
			i++;
		}
	}
}

void update_doors(t_game *g)
{
	int i = 0;
	while (i < g->map->door_count)
	{
		if (g->map->doors[i].is_open)
		{
			g->map->doors[i].open_timer += g->frame_time;
			/* Auto-close doors after 3 seconds */
			if (g->map->doors[i].open_timer > 3.0)
			{
				g->map->doors[i].is_open = false;
				g->map->doors[i].open_timer = 0.0;
			}
		}
		i++;
	}
}

/* Enhanced movement with proper collision detection */
static void apply_movement_bonus(t_game *g)
{
	double new_x, new_y;
	double collision_buffer = 0.1; /* Small buffer to prevent getting stuck in walls */
	
	if (g->keys.w)
	{
		new_x = g->pos_x + g->dir_x * g->move_speed;
		new_y = g->pos_y + g->dir_y * g->move_speed;
		
		/* Check X movement */
		if (!check_collision(g, new_x + (g->dir_x > 0 ? collision_buffer : -collision_buffer), g->pos_y))
			g->pos_x = new_x;
		/* Check Y movement */
		if (!check_collision(g, g->pos_x, new_y + (g->dir_y > 0 ? collision_buffer : -collision_buffer)))
			g->pos_y = new_y;
	}
	if (g->keys.s)
	{
		new_x = g->pos_x - g->dir_x * g->move_speed;
		new_y = g->pos_y - g->dir_y * g->move_speed;
		
		/* Check X movement */
		if (!check_collision(g, new_x + (g->dir_x < 0 ? collision_buffer : -collision_buffer), g->pos_y))
			g->pos_x = new_x;
		/* Check Y movement */
		if (!check_collision(g, g->pos_x, new_y + (g->dir_y < 0 ? collision_buffer : -collision_buffer)))
			g->pos_y = new_y;
	}
	if (g->keys.a)
	{
		new_x = g->pos_x - g->plane_x * g->move_speed;
		new_y = g->pos_y - g->plane_y * g->move_speed;
		
		/* Check X movement */
		if (!check_collision(g, new_x + (g->plane_x < 0 ? collision_buffer : -collision_buffer), g->pos_y))
			g->pos_x = new_x;
		/* Check Y movement */
		if (!check_collision(g, g->pos_x, new_y + (g->plane_y < 0 ? collision_buffer : -collision_buffer)))
			g->pos_y = new_y;
	}
	if (g->keys.d)
	{
		new_x = g->pos_x + g->plane_x * g->move_speed;
		new_y = g->pos_y + g->plane_y * g->move_speed;
		
		/* Check X movement */
		if (!check_collision(g, new_x + (g->plane_x > 0 ? collision_buffer : -collision_buffer), g->pos_y))
			g->pos_x = new_x;
		/* Check Y movement */
		if (!check_collision(g, g->pos_x, new_y + (g->plane_y > 0 ? collision_buffer : -collision_buffer)))
			g->pos_y = new_y;
	}
	
	/* Keyboard rotation */
	if (g->keys.left || g->keys.right)
	{
		double rot = g->rot_speed * (g->keys.left ? 1.0 : -1.0);
		double old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		double old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
}

/* Mouse rotation handling */
int on_mouse_move(int x, int y, void *param)
{
	t_game *g = (t_game *)param;
	
	/* If mouse ventures too close to edges, recenter to maintain continuous rotation */
	if (x < 50 || x > WIN_W - 50 || y < 50 || y > WIN_H - 50)
	{
		mlx_mouse_move(g->mlx, g->win, WIN_W / 2, WIN_H / 2);
		g->mouse.last_x = WIN_W / 2;
		g->mouse.last_y = WIN_H / 2;
		g->mouse.x = WIN_W / 2;
		g->mouse.y = WIN_H / 2;
		g->mouse.captured = true;
		return (0);
	}
	
	if (!g->mouse.captured)
	{
		g->mouse.x = x;
		g->mouse.y = y;
		g->mouse.last_x = x;
		g->mouse.last_y = y;
		g->mouse.captured = true;
		return (0);
	}
	
	int dx = x - g->mouse.last_x;
	double sensitivity = 0.002; /* Mouse sensitivity */
	double rot = dx * sensitivity;
	
	if (iabs(dx) > 1) /* Only rotate if minimal significant movement */
	{
		double old_dir_x = g->dir_x;
		g->dir_x = g->dir_x * cos(rot) - g->dir_y * sin(rot);
		g->dir_y = old_dir_x * sin(rot) + g->dir_y * cos(rot);
		double old_plane_x = g->plane_x;
		g->plane_x = g->plane_x * cos(rot) - g->plane_y * sin(rot);
		g->plane_y = old_plane_x * sin(rot) + g->plane_y * cos(rot);
	}
	
	g->mouse.last_x = x;
	g->mouse.last_y = y;
	g->mouse.x = x;
	g->mouse.y = y;
	
	return (0);
}

/* Input hooks for bonus features */
int on_key_press_bonus(int keycode, void *param)
{
	t_game *g = (t_game *)param;
	if (keycode == KEY_ESC)
		mlx_loop_end(g->mlx);
	else if (keycode == KEY_W) g->keys.w = 1;
	else if (keycode == KEY_A) g->keys.a = 1;
	else if (keycode == KEY_S) g->keys.s = 1;
	else if (keycode == KEY_D) g->keys.d = 1;
	else if (keycode == KEY_LEFT) g->keys.left = 1;
	else if (keycode == KEY_RIGHT) g->keys.right = 1;
	else if (keycode == KEY_SPACE) 
	{
		g->keys.space = 1;
		handle_door_interaction(g);
	}
	return (0);
}

int on_key_release_bonus(int keycode, void *param)
{
	t_game *g = (t_game *)param;
	if (keycode == KEY_W) g->keys.w = 0;
	else if (keycode == KEY_A) g->keys.a = 0;
	else if (keycode == KEY_S) g->keys.s = 0;
	else if (keycode == KEY_D) g->keys.d = 0;
	else if (keycode == KEY_LEFT) g->keys.left = 0;
	else if (keycode == KEY_RIGHT) g->keys.right = 0;
	else if (keycode == KEY_SPACE) g->keys.space = 0;
	return (0);
}

int on_destroy_bonus(void *param)
{
	t_game *g = (t_game *)param;
	mlx_loop_end(g->mlx);
	return (0);
}

/* Main loop callback for bonus features */
int game_loop_bonus(void *param)
{
	t_game *g = (t_game *)param;
	
	/* Update frame time */
	g->frame_time = get_time_delta(&g->last_time);
	
	/* Update game state */
	apply_movement_bonus(g);
	update_doors(g);
	update_sprites(g);
	
	/* Render the frame */
	render_frame_bonus(g);
	return (0);
}
