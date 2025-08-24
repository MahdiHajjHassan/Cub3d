#include "cub3d.h"
#include "mlx.h"

/* X11 keycodes */
#define KEY_ESC   65307
#define KEY_LEFT  65361
#define KEY_RIGHT 65363
#define KEY_W     119
#define KEY_A     97
#define KEY_S     115
#define KEY_D     100

/* Forward declarations implemented in other render files */
int  frame_init(t_game *g);
void frame_destroy(t_game *g);
int  textures_load(t_game *g, const t_config *cfg);
void textures_destroy(t_game *g);
int  on_key_press(int keycode, void *param);
int  on_key_release(int keycode, void *param);
int  on_destroy(void *param);
int  game_loop(void *param);

/* Initialize game state from parsed config */
static void init_game_from_cfg(t_game *g, const t_config *cfg)
{
	g->map = &cfg->map;
	g->pos_x = (double)cfg->map.player_x + 0.5;
	g->pos_y = (double)cfg->map.player_y + 0.5;
	g->dir_x = cos(cfg->map.player_angle);
	g->dir_y = -sin(cfg->map.player_angle);
	/* 66° FOV -> plane length ~ tan(fov/2) ~ 0.66 */
	g->plane_x = -g->dir_y * 0.66;
	g->plane_y = g->dir_x * 0.66;
	g->keys = (t_keys){0,0,0,0,0,0};
	g->move_speed = 0.06;
	g->rot_speed = 0.04;
	g->ceil_color = cfg->ceiling_color.value;
	g->floor_color = cfg->floor_color.value;
}

int run_game(const t_config *cfg)
{
	t_game g;

	/* Zero-initialize */
	g.mlx = NULL; g.win = NULL; g.frame = (t_img){0};
	g.tex[0] = (t_img){0}; g.tex[1] = (t_img){0}; g.tex[2] = (t_img){0}; g.tex[3] = (t_img){0};

	g.mlx = mlx_init();
	if (!g.mlx)
		return (error_msg("mlx: init failed"));
	g.win = mlx_new_window(g.mlx, WIN_W, WIN_H, "cub3d");
	if (!g.win)
		return (mlx_destroy_display(g.mlx), free(g.mlx), error_msg("mlx: window failed"));
	if (frame_init(&g) != 0)
		return (mlx_destroy_window(g.mlx, g.win), mlx_destroy_display(g.mlx), free(g.mlx), 1);
	init_game_from_cfg(&g, cfg);
	if (textures_load(&g, cfg) != 0)
	{
		frame_destroy(&g);
		mlx_destroy_window(g.mlx, g.win);
		mlx_destroy_display(g.mlx);
		free(g.mlx);
		return (1);
	}
	/* Install hooks */
	mlx_hook(g.win, 2, 1L<<0, &on_key_press, &g);    /* KeyPress */
	mlx_hook(g.win, 3, 1L<<1, &on_key_release, &g);  /* KeyRelease */
	mlx_hook(g.win, 17, 0, &on_destroy, &g);         /* DestroyNotify */
	mlx_loop_hook(g.mlx, &game_loop, &g);
	/* Enter event loop */
	mlx_loop(g.mlx);
	/* Cleanup (in case of loop_end) */
	textures_destroy(&g);
	frame_destroy(&g);
	mlx_destroy_window(g.mlx, g.win);
	mlx_destroy_display(g.mlx);
	free(g.mlx);
	return (0);
}


