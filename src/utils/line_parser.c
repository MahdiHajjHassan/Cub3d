/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:00:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*substr_dup(const char *src, size_t start, size_t length)
{
	char	*out;
	size_t	i;

	out = malloc(length + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < length)
	{
		out[i] = src[start + i];
		i++;
	}
	out[length] = '\0';
	return (out);
}

static void	*free_partial(char **lines, size_t count)
{
	free_lines(lines, count);
	return (NULL);
}

size_t	count_lines(const char *content, size_t len)
{
	size_t	count;
	size_t	i;

	if (len == 0)
		return (0);
	count = 0;
	i = 0;
	while (i < len)
	{
		if (content[i] == '\n')
			count++;
		i++;
	}
	if (len > 0 && content[len - 1] != '\n')
		count++;
	return (count);
}

static int	process_line(const char *content, char ***lines, size_t *vars)
{
	(*lines)[vars[2]] = substr_dup(content, vars[1], vars[0] - vars[1]);
	if (!(*lines)[vars[2]])
		return (free_partial(*lines, vars[2]), error_msg("malloc"));
	vars[2]++;
	vars[1] = vars[0] + 1;
	return (0);
}

int	split_lines(const char *content, size_t len, char ***lines)
{
	size_t	vars[3];

	*lines = malloc(sizeof(char *) * (count_lines(content, len) + 1));
	if (!*lines)
		return (error_msg("malloc failed"));
	vars[0] = 0;
	vars[1] = 0;
	vars[2] = 0;
	while (vars[0] < len)
	{
		if (content[vars[0]] == '\n')
		{
			if (process_line(content, lines, vars))
				return (1);
		}
		vars[0]++;
	}
	if (vars[1] < len)
	{
		if (process_line(content, lines, vars))
			return (1);
	}
	(*lines)[vars[2]] = NULL;
	return (0);
}
