/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader_new.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 20:17:19 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	read_loop(int fd, t_file_buffer *buf)
{
	char	temp[4096];
	ssize_t	bytes;

	bytes = read(fd, temp, sizeof(temp));
	while (bytes > 0)
	{
		if (append_to_buffer(buf, temp, bytes))
		{
			free_buffer(buf);
			return (1);
		}
		bytes = read(fd, temp, sizeof(temp));
	}
	if (bytes < 0)
	{
		free_buffer(buf);
		return (1);
	}
	return (0);
}

static int	read_file_to_buffer(const char *path, t_file_buffer *buf)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg("cannot open file"));
	if (init_buffer(buf, 8192))
		return (close(fd), error_msg("malloc failed"));
	if (read_loop(fd, buf))
		return (close(fd), error_msg("read/malloc failed"));
	close(fd);
	return (0);
}

int	read_all_lines(const char *path, char ***out_lines, size_t *out_count)
{
	t_file_buffer	buf;

	if (!out_lines || !out_count)
		return (error_msg("internal: invalid args"));
	*out_lines = NULL;
	*out_count = 0;
	if (read_file_to_buffer(path, &buf))
		return (1);
	*out_count = count_lines(buf.content, buf.len);
	if (split_lines(buf.content, buf.len, out_lines))
	{
		free_buffer(&buf);
		return (1);
	}
	free_buffer(&buf);
	return (0);
}
