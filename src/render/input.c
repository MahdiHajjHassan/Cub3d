#include "cub3d.h"
#include "mlx.h"

/* NOTE: This file only provides the hook wrapper symbols if subject enforces
** file-per-feature separation. The actual implementations are in engine.c
** because we keep the loop logic together. */

int on_key_press(int keycode, void *param);
int on_key_release(int keycode, void *param);
int on_destroy(void *param);
int game_loop(void *param);


