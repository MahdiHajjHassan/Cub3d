/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 18:57:12 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "render.h"
#include <sys/time.h>

double	get_time_delta(struct timeval *last_time)
{
	struct timeval	current_time;
	double			delta;

	gettimeofday(&current_time, NULL);
	delta = (current_time.tv_sec - last_time->tv_sec)
		+ (current_time.tv_usec - last_time->tv_usec) / 1000000.0;
	*last_time = current_time;
	return (delta);
}
