// Minimal renderer: open window, draw ceiling/floor colors, wait for ESC/close.
#include "cub3d.h"
#include "mlx.h"

typedef struct s_mlx
{
    void    *mlx;
    void    *win;
    void    *img;
    char    *img_data;
    int     bpp;
    int     size_line;
    int     endian;
    int     width;
    int     height;
    int     ceil_color;
    int     floor_color;
}   t_mlx;

// Close helper: end loop (used by hooks)
static int  end_loop(void *param)
{
    t_mlx *ctx;

    ctx = (t_mlx *)param;
    // Tell MLX main loop to exit
    if (ctx && ctx->mlx)
        mlx_loop_end(ctx->mlx);
    return (0);
}

// Key hook: ESC to quit
static int  on_key(int keycode, void *param)
{
    // ESC key (XK_Escape) ends the loop
    if (keycode == 65307)
        return (end_loop(param));
    return (0);
}

// Create window and 2D image buffer
static int  setup_window_and_image(t_mlx *ctx)
{
    ctx->width = 800;
    ctx->height = 600;
    // Connect to X server
    ctx->mlx = mlx_init();
    if (!ctx->mlx)
        return (error_msg("mlx: init failed"));
    // Create the application window
    ctx->win = mlx_new_window(ctx->mlx, ctx->width, ctx->height, "cub3d");
    if (!ctx->win)
        return (mlx_destroy_display(ctx->mlx), free(ctx->mlx), error_msg("mlx: window failed"));
    // Create an off-screen image buffer
    ctx->img = mlx_new_image(ctx->mlx, ctx->width, ctx->height);
    if (!ctx->img)
        return (mlx_destroy_window(ctx->mlx, ctx->win), mlx_destroy_display(ctx->mlx), free(ctx->mlx), error_msg("mlx: image failed"));
    // Access pixel buffer metadata
    ctx->img_data = mlx_get_data_addr(ctx->img, &ctx->bpp, &ctx->size_line, &ctx->endian);
    if (!ctx->img_data)
        return (mlx_destroy_image(ctx->mlx, ctx->img), mlx_destroy_window(ctx->mlx, ctx->win), mlx_destroy_display(ctx->mlx), free(ctx->mlx), error_msg("mlx: data addr failed"));
    return (0);
}

// Fill the image: top half ceiling, bottom half floor
static void draw_background(t_mlx *ctx)
{
    int y;
    int x;
    int bytes;
    int color;
    char *p;

    bytes = ctx->bpp / 8;
    y = 0;
    while (y < ctx->height)
    {
        // Choose top-half (ceiling) or bottom-half (floor) color
        color = (y < ctx->height / 2) ? ctx->ceil_color : ctx->floor_color;
        x = 0;
        while (x < ctx->width)
        {
            // Compute pointer to pixel (y, x)
            p = ctx->img_data + (y * ctx->size_line) + (x * bytes);
            if (bytes >= 4)
            {
                // MLX is BGRA or similar; write B,G,R, then alpha
                p[0] = (char)(color & 0xFF);
                p[1] = (char)((color >> 8) & 0xFF);
                p[2] = (char)((color >> 16) & 0xFF);
                p[3] = 0;
            }
            x++;
        }
        y++;
    }
}

// Release MLX resources
static void teardown(t_mlx *ctx)
{
    if (!ctx)
        return;
    if (ctx->mlx && ctx->img)
        mlx_destroy_image(ctx->mlx, ctx->img);
    if (ctx->mlx && ctx->win)
        mlx_destroy_window(ctx->mlx, ctx->win);
    if (ctx->mlx)
    {
        mlx_destroy_display(ctx->mlx);
        free(ctx->mlx);
    }
}

int run_game(const t_config *cfg)
{
    t_mlx   ctx;

    ctx.mlx = NULL; ctx.win = NULL; ctx.img = NULL; ctx.img_data = NULL;
    // Create window + image buffer
    if (setup_window_and_image(&ctx) != 0)
        return (1);
    // Convert RGB from config into MLX-native color values
    ctx.ceil_color = mlx_get_color_value(ctx.mlx, cfg->ceiling_color.value);
    ctx.floor_color = mlx_get_color_value(ctx.mlx, cfg->floor_color.value);
    // Paint the background once
    draw_background(&ctx);
    // Blit image to window and install basic hooks
    mlx_put_image_to_window(ctx.mlx, ctx.win, ctx.img, 0, 0);
    mlx_key_hook(ctx.win, &on_key, &ctx);
    mlx_hook(ctx.win, 17, 0, &end_loop, &ctx);
    // Enter event loop until user closes or presses ESC
    mlx_loop(ctx.mlx);
    // Free all resources cleanly
    teardown(&ctx);
    return (0);
}


