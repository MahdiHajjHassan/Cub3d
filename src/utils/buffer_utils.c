/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 20:14:04 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*grow_buffer(char *old, size_t old_len, size_t new_cap)
{
	char	*new_buf;
	size_t	i;

	new_buf = malloc(new_cap);
	if (!new_buf)
		return (NULL);
	i = 0;
	while (i < old_len)
	{
		new_buf[i] = old[i];
		i++;
	}
	free(old);
	return (new_buf);
}

int	init_buffer(t_file_buffer *buf, size_t initial_cap)
{
	buf->content = malloc(initial_cap);
	if (!buf->content)
		return (1);
	buf->len = 0;
	buf->cap = initial_cap;
	return (0);
}

int	append_to_buffer(t_file_buffer *buf, char *data, size_t bytes)
{
	while (buf->len + bytes >= buf->cap)
		buf->cap *= 2;
	if (buf->len + bytes >= buf->cap)
	{
		buf->content = grow_buffer(buf->content, buf->len, buf->cap);
		if (!buf->content)
			return (1);
	}
	while (bytes--)
		buf->content[buf->len++] = *data++;
	return (0);
}

void	free_buffer(t_file_buffer *buf)
{
	if (buf->content)
		free(buf->content);
	buf->content = NULL;
	buf->len = 0;
	buf->cap = 0;
}
