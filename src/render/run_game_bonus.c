#include "cub3d_bonus.h"
#include "mlx.h"
#include <X11/Xlib.h>

static int	display_available(void)
{
	/* Try an actual XOpenDisplay to verify connection works */
	Display	*dpy;

	dpy = XOpenDisplay(NULL);
	if (!dpy)
		return (0);
	XCloseDisplay(dpy);
	return (1);
}

/* Forward declarations implemented in other bonus render files */
int  frame_init(t_game *g);
void frame_destroy(t_game *g);
int  textures_load(t_game *g, const t_config *cfg);
void textures_destroy(t_game *g);
int  on_key_press_bonus(int keycode, void *param);
int  on_key_release_bonus(int keycode, void *param);
int  on_destroy_bonus(void *param);
int  on_mouse_move(int x, int y, void *param);
int  game_loop_bonus(void *param);
int  init_minimap(t_game *g);
void destroy_minimap(t_game *g);

/* Initialize game state from parsed config */
static void init_game_from_cfg_bonus(t_game *g, const t_config *cfg)
{
	g->map = &cfg->map;
	g->pos_x = (double)cfg->map.player_x + 0.5;
	g->pos_y = (double)cfg->map.player_y + 0.5;
	g->dir_x = cos(cfg->map.player_angle);
	g->dir_y = -sin(cfg->map.player_angle);
	/* 66° FOV -> plane length ~ tan(fov/2) ~ 0.66 */
	g->plane_x = -g->dir_y * 0.66;
	g->plane_y = g->dir_x * 0.66;
	g->keys = (t_keys){0,0,0,0,0,0,0};
	g->mouse = (t_mouse){0,0,0,0,false};
	g->move_speed = 0.06;
	g->rot_speed = 0.04;
	g->ceil_color = cfg->ceiling_color.value;
	g->floor_color = cfg->floor_color.value;
	g->closing = 0;
	/* Initialize timing */
	gettimeofday(&g->last_time, NULL);
	g->frame_time = 0.0;
}

int run_game(const t_config *cfg)
{
	t_game g;
	int i;

write(2, "DBG: run_game start\n", sizeof("DBG: run_game start\n") - 1);
	if (!display_available())
		return (error_msg("no DISPLAY; run under X11 or use xvfb-run"));

	/* Zero-initialize */
	g.mlx = NULL; 
	g.win = NULL; 
	g.frame = (t_img){0};
	g.minimap = (t_minimap){0};
	
	/* Zero-initialize all textures */
	i = 0;
	while (i < TEX_COUNT + MAX_SPRITES * MAX_SPRITE_FRAMES)
	{
		g.tex[i] = (t_img){0};
		i++;
	}

write(2, "DBG: mlx_init\n", sizeof("DBG: mlx_init\n") - 1);
	g.mlx = mlx_init();
	if (!g.mlx)
		return (error_msg("mlx: init failed"));
write(2, "DBG: new_window\n", sizeof("DBG: new_window\n") - 1);
	g.win = mlx_new_window(g.mlx, WIN_W, WIN_H, "cub3d - Bonus");
	if (!g.win)
		return (mlx_destroy_display(g.mlx), free(g.mlx), error_msg("mlx: window failed"));
write(2, "DBG: frame_init\n", sizeof("DBG: frame_init\n") - 1);
	if (frame_init(&g) != 0)
		return (mlx_destroy_window(g.mlx, g.win), mlx_destroy_display(g.mlx), free(g.mlx), 1);
	
write(2, "DBG: init_game_from_cfg_bonus\n", sizeof("DBG: init_game_from_cfg_bonus\n") - 1);
	init_game_from_cfg_bonus(&g, cfg);
	
write(2, "DBG: textures_load\n", sizeof("DBG: textures_load\n") - 1);
	if (textures_load(&g, cfg) != 0)
	{
		frame_destroy(&g);
		mlx_destroy_window(g.mlx, g.win);
		mlx_destroy_display(g.mlx);
		free(g.mlx);
		return (1);
	}
	
	/* Initialize minimap */
write(2, "DBG: init_minimap\n", sizeof("DBG: init_minimap\n") - 1);
	if (init_minimap(&g) != 0)
	{
		textures_destroy(&g);
		frame_destroy(&g);
		mlx_destroy_window(g.mlx, g.win);
		mlx_destroy_display(g.mlx);
		free(g.mlx);
		return (1);
	}
	
	/* Install hooks for bonus features */
write(2, "DBG: install hooks\n", sizeof("DBG: install hooks\n") - 1);
	mlx_hook(g.win, 2, 1L<<0, &on_key_press_bonus, &g);    /* KeyPress */
	mlx_hook(g.win, 3, 1L<<1, &on_key_release_bonus, &g);  /* KeyRelease */
	mlx_hook(g.win, 17, 0, &on_destroy_bonus, &g);         /* DestroyNotify */
	mlx_hook(g.win, 6, 1L<<6, &on_mouse_move, &g);         /* MotionNotify for mouse */
	mlx_loop_hook(g.mlx, &game_loop_bonus, &g);

	/* Hide cursor and center mouse to start for smooth rotation */
write(2, "DBG: mouse hide/move\n", sizeof("DBG: mouse hide/move\n") - 1);
	if (g.mlx && g.win)
	{
		mlx_mouse_hide(g.mlx, g.win);
		mlx_mouse_move(g.mlx, g.win, WIN_W / 2, WIN_H / 2);
	}
	g.mouse.captured = true;
	g.mouse.x = WIN_W / 2; g.mouse.last_x = WIN_W / 2;
	g.mouse.y = WIN_H / 2; g.mouse.last_y = WIN_H / 2;
	
	/* Enter event loop */
write(2, "DBG: entering loop\n", sizeof("DBG: entering loop\n") - 1);
	mlx_loop(g.mlx);
write(2, "DBG: left loop\n", sizeof("DBG: left loop\n") - 1);
	
	/* Cleanup (in case of loop_end) */
	if (g.mlx && g.win)
		mlx_mouse_show(g.mlx, g.win);
	destroy_minimap(&g);
	textures_destroy(&g);
	frame_destroy(&g);
	mlx_destroy_window(g.mlx, g.win);
	mlx_destroy_display(g.mlx);
	free(g.mlx);
	
	return (0);
}
