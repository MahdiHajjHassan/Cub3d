/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:30:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:45:43 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <X11/Xlib.h>

int	display_available(void)
{
	Display	*dpy;

	dpy = XOpenDisplay(NULL);
	if (!dpy)
		return (0);
	XCloseDisplay(dpy);
	return (1);
}
