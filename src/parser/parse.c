/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:49:09 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/13 13:25:44 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	parse_cub_file(const char *path, t_config *out_cfg)
{
	char			**lines;
	size_t			count;
	t_parse_flags	flags;
	size_t			map_start;
	t_parse_context	ctx;

	map_start = 0;
	flags = init_flags();
	init_config(out_cfg);
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	ctx.lines = lines;
	ctx.count = count;
	ctx.cfg = out_cfg;
	ctx.flags = &flags;
	ctx.map_start = &map_start;
	if (parse_headers(&ctx) != 0)
		return (1);
	if (validate_and_parse_map(&ctx, map_start) != 0)
		return (1);
	free_lines(lines, count);
	return (0);
}
