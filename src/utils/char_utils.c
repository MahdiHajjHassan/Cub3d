/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 00:00:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/14 20:14:04 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

int	is_digit(int c)
{
	return (c >= '0' && c <= '9');
}

size_t	skip_chars(const char *s, size_t i, int (*check)(int))
{
	while (s[i] && check((unsigned char)s[i]))
		i++;
	return (i);
}
