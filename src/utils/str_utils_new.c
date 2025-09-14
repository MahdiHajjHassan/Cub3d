/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:39:29 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 20:14:04 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*copy_trimmed(const char *s, size_t start, size_t end)
{
	char	*out;
	size_t	i;

	out = malloc(end - start + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		out[i] = s[start + i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

char	*str_trim_spaces(const char *s)
{
	size_t	start;
	size_t	end;

	if (!s)
		return (NULL);
	start = 0;
	while (s[start] && is_space((unsigned char)s[start]))
		start++;
	end = start;
	while (s[end])
		end++;
	while (end > start && is_space((unsigned char)s[end - 1]))
		end--;
	return (copy_trimmed(s, start, end));
}

int	starts_with(const char *s, const char *prefix)
{
	size_t	i;

	if (!s || !prefix)
		return (0);
	i = 0;
	while (prefix[i])
	{
		if (s[i] != prefix[i])
			return (0);
		i++;
	}
	return (1);
}
