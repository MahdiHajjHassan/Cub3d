/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:39:26 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 20:14:04 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_lines(char **lines, size_t count)
{
	size_t	i;

	if (!lines)
		return ;
	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}
