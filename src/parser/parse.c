/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:49:09 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:11:49 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

static int	setup_parse_context(t_parse_context *ctx, char **lines,
	size_t count, t_config *out_cfg)
{
	static t_parse_flags	flags;
	static size_t			map_start;

	map_start = 0;
	flags = init_flags();
	ctx->lines = lines;
	ctx->count = count;
	ctx->cfg = out_cfg;
	ctx->flags = &flags;
	ctx->map_start = &map_start;
	return (0);
}

static int	parse_with_cleanup(t_parse_context *ctx, char **lines, size_t count)
{
	if (parse_headers(ctx) != 0)
	{
		free_config(ctx->cfg);
		free_lines(lines, count);
		return (1);
	}
	if (validate_and_parse_map(ctx, *(ctx->map_start)) != 0)
	{
		free_config(ctx->cfg);
		free_lines(lines, count);
		return (1);
	}
	return (0);
}

int	parse_cub_file(const char *path, t_config *out_cfg)
{
	char			**lines;
	size_t			count;
	t_parse_context	ctx;

	init_config(out_cfg);
	if (read_all_lines(path, &lines, &count) != 0)
		return (1);
	setup_parse_context(&ctx, lines, count, out_cfg);
	if (parse_with_cleanup(&ctx, lines, count) != 0)
		return (1);
	free_lines(lines, count);
	return (0);
}
