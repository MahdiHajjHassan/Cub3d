/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_headers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:15:00 by hsharaf-          #+#    #+#             */
/*   Updated: 2025/09/16 15:00:01 by hsharaf-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	process_single_header(t_parse_context *ctx, char *trimmed)
{
	t_header_context	header_ctx;
	int					result;

	header_ctx.trimmed = trimmed;
	header_ctx.lines = ctx->lines;
	header_ctx.count = ctx->count;
	header_ctx.out_cfg = ctx->cfg;
	header_ctx.flags = ctx->flags;
	result = process_header_line(&header_ctx);
	if (result == 1)
		return (1);
	if (result == 2)
		return (2);
	return (0);
}

int	parse_single_line(t_parse_context *ctx)
{
	char	*trimmed;
	int		result;

	trimmed = str_trim_spaces(ctx->lines[*(ctx->map_start)]);
	if (!trimmed)
		return (error_msg("parse: memory error"));
	if (handle_empty_line(trimmed, ctx->map_start))
		return (0);
	result = process_single_header(ctx, trimmed);
	if (result == 1 || result == 2)
	{
		return (result);
	}
	free(trimmed);
	(*(ctx->map_start))++;
	return (0);
}

int	parse_headers(t_parse_context *ctx)
{
	int	result;

	while (*(ctx->map_start) < ctx->count)
	{
		result = parse_single_line(ctx);
		if (result == -1)
			return (-1);
		if (result == 1)
			return (1);
		if (result == 2)
			break ;
	}
	return (0);
}
