/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 23:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 23:39:19 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

void	center_window_on_screen(void *mlx_ptr, void *win_ptr)
{
	Display	*display;
	int		screen_width;
	int		screen_height;
	int		center_x;
	int		center_y;

	if (!mlx_ptr || !win_ptr)
		return ;
	display = XOpenDisplay(NULL);
	if (!display)
		return ;
	screen_width = XDisplayWidth(display, DefaultScreen(display));
	screen_height = XDisplayHeight(display, DefaultScreen(display));
	center_x = (screen_width - WIN_W) / 2;
	center_y = (screen_height - WIN_H) / 2;
	if (center_x < 0)
		center_x = 0;
	if (center_y < 0)
		center_y = 0;
	setenv("WINDOWID", (char *)win_ptr, 1);
	XCloseDisplay(display);
}
