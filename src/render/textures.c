/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:22:01 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 18:22:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"
#include "mlx.h"

/* Load one texture from xpm file path into t_img */
static int load_one(void *mlx, const char *path, t_img *out)
{
	int w, h;

	out->img = mlx_xpm_file_to_image(mlx, (char *)path, &w, &h);
	if (!out->img)
		return (error_msg("mlx: xpm load failed"));
	out->data = mlx_get_data_addr(out->img, &out->bpp, &out->line_len, &out->endian);
	if (!out->data)
		return (mlx_destroy_image(mlx, out->img), *out=(t_img){0}, error_msg("mlx: tex data addr failed"));
	out->width = w;
	out->height = h;
	return (0);
}

int textures_load(t_game *g, const t_config *cfg)
{
if (load_one(g->mlx, cfg->textures.no, &g->tex[TEX_NO]) != 0)
		return (1);
	if (load_one(g->mlx, cfg->textures.so, &g->tex[TEX_SO]) != 0)
		return (mlx_destroy_image(g->mlx, g->tex[TEX_NO].img), g->tex[TEX_NO]=(t_img){0}, 1);
	if (load_one(g->mlx, cfg->textures.ea, &g->tex[TEX_EA]) != 0)
	{
		mlx_destroy_image(g->mlx, g->tex[TEX_NO].img); g->tex[TEX_NO]=(t_img){0};
		mlx_destroy_image(g->mlx, g->tex[TEX_SO].img); g->tex[TEX_SO]=(t_img){0};
		return (1);
	}
	if (load_one(g->mlx, cfg->textures.we, &g->tex[TEX_WE]) != 0)
	{
		mlx_destroy_image(g->mlx, g->tex[TEX_NO].img); g->tex[TEX_NO]=(t_img){0};
		mlx_destroy_image(g->mlx, g->tex[TEX_SO].img); g->tex[TEX_SO]=(t_img){0};
		mlx_destroy_image(g->mlx, g->tex[TEX_EA].img); g->tex[TEX_EA]=(t_img){0};
		return (1);
	}
	return (0);
}

void textures_destroy(t_game *g)
{
	int i = 0;
	while (i < TEX_COUNT)
	{
		if (g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i] = (t_img){0};
		i++;
	}
}

