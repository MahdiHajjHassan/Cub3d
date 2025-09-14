/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_center.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 23:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:39:19 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include "mlx.h"

static void	move_window_to_center(t_xvar *xvar, t_win_list *win)
{
	int	screen_width;
	int	screen_height;
	int	center_x;
	int	center_y;

	screen_width = XDisplayWidth(xvar->display, xvar->screen);
	screen_height = XDisplayHeight(xvar->display, xvar->screen);
	center_x = (screen_width - WIN_W) / 2;
	center_y = (screen_height - WIN_H) / 2;
	if (center_x < 0)
		center_x = 0;
	if (center_y < 0)
		center_y = 0;
	XMoveWindow(xvar->display, win->window, center_x, center_y);
	XFlush(xvar->display);
}

void	center_window_on_screen(void *mlx_ptr, void *win_ptr)
{
	if (!mlx_ptr || !win_ptr)
		return ;
	move_window_to_center((t_xvar *)mlx_ptr, (t_win_list *)win_ptr);
}
