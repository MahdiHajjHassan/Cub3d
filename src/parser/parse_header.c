/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:06:55 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/08/24 21:06:55 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"
#include "parser.h"

/* RGB parsing moved out from parse_utils to reduce function count */
static int	read_u8(const char *s, size_t *i, int *out)
{
	size_t	k;
	int		val;

	*i = skip_spaces(s, *i);
	k = *i;
	if (!is_digit((unsigned char)s[k]))
		return (error_msg("color: expected digit"));
	val = 0;
	while (s[k] && is_digit((unsigned char)s[k]))
	{
		val = val * 10 + (s[k] - '0');
		k++;
	}
	if (val < 0 || val > 255)
		return (error_msg("color: out of range"));
	*out = val;
	*i = k;
	return (0);
}

int	parse_rgb_triplet(const char *s, t_color *c)
{
	int		r;
	int		g;
	int		b;
	size_t	i;

	i = 0;
	if (read_u8(s, &i, &r) != 0)
		return (1);
	i = skip_spaces(s, i);
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &g) != 0)
		return (1);
	i = skip_spaces(s, i);
	if (s[i] != ',')
		return (error_msg("color: expected comma"));
	i++;
	if (read_u8(s, &i, &b) != 0)
		return (1);
	c->r = r;
	c->g = g;
	c->b = b;
	c->value = (r << 16) | (g << 8) | b;
	return (0);
}

