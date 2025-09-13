# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsharaf- <hsharaf-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/10 15:39:36 by hsharaf-          #+#    #+#              #
#    Updated: 2025/09/13 23:28:50 by hsharaf-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := cub3d

SRC_DIR := src
INC_DIR := include
LIBFT_DIR := $(INC_DIR)/libft
MLX_DIR := minilibx-linux

# Optional debugging (make DEBUG=1)
DEBUG ?= 0

LIBFT := $(LIBFT_DIR)/libft.a
MLX := $(MLX_DIR)/libmlx.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parser/parse.c \
	$(SRC_DIR)/parser/parse_config.c \
	$(SRC_DIR)/parser/parse_colors.c \
	$(SRC_DIR)/parser/parse_sprites.c \
	$(SRC_DIR)/parser/parse_map.c \
	$(SRC_DIR)/parser/parse_map_utils.c \
	$(SRC_DIR)/parser/parse_map_validation.c \
	$(SRC_DIR)/parser/parse_headers.c \
	$(SRC_DIR)/parser/parse_textures.c \
	$(SRC_DIR)/parser/parse_validation.c \
	$(SRC_DIR)/render/game/run_game.c \
	$(SRC_DIR)/render/game/game_display.c \
	$(SRC_DIR)/render/game/game_init.c \
	$(SRC_DIR)/render/game/game_setup.c \
	$(SRC_DIR)/render/game/game_cleanup.c \
	$(SRC_DIR)/render/engine/engine.c \
	$(SRC_DIR)/render/engine/frame.c \
	$(SRC_DIR)/render/engine/drawing.c \
	$(SRC_DIR)/render/engine/raycasting.c \
	$(SRC_DIR)/render/engine/wall.c \
	$(SRC_DIR)/render/input.c \
	$(SRC_DIR)/render/movement/movement.c \
	$(SRC_DIR)/render/movement/movement_forward.c \
	$(SRC_DIR)/render/movement/movement_backward.c \
	$(SRC_DIR)/render/movement/movement_strafe.c \
	$(SRC_DIR)/render/movement/movement_rotation.c \
	$(SRC_DIR)/render/input_mouse.c \
	$(SRC_DIR)/render/input_doors.c \
	$(SRC_DIR)/render/collision.c \
	$(SRC_DIR)/render/textures.c \
	$(SRC_DIR)/render/sprites/sprites.c \
	$(SRC_DIR)/render/sprites/sprites_time.c \
	$(SRC_DIR)/render/sprites/sprites_update.c \
	$(SRC_DIR)/render/sprites/sprites_sort.c \
	$(SRC_DIR)/render/sprites/sprites_render.c \
	$(SRC_DIR)/render/sprites/sprites_single.c \
	$(SRC_DIR)/render/sprites/sprites_column.c \
	$(SRC_DIR)/render/sprites/sprites_pixel.c \
	$(SRC_DIR)/render/minimap/minimap.c \
	$(SRC_DIR)/render/minimap/minimap_render.c \
	$(SRC_DIR)/render/minimap/minimap_main.c \
	$(SRC_DIR)/render/minimap/minimap_player.c \
	$(SRC_DIR)/render/minimap/minimap_direction.c \
	$(SRC_DIR)/utils/file_utils.c \
	$(SRC_DIR)/utils/str_utils.c \
	$(SRC_DIR)/utils/error.c

OBJS := $(SRCS:.c=.o)

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)
LDLIBS := $(LIBFT) $(MLX) -lXext -lX11 -lm

ifeq ($(DEBUG),1)
CFLAGS += -g3 -O0
endif

# Colors
GREEN := \033[1;32m
RED := \033[1;31m
RST := \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME) > /dev/null 2>&1 || (printf '$(RED)Compilation failed — remove quiet to see errors$(RST)\n' && false)
	@printf '$(GREEN)%s compiled successfully$(RST)\n' $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null 2>&1

libft: $(LIBFT)
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1

mlx: $(MLX)
$(MLX):
	@$(MAKE) -C $(MLX_DIR) > /dev/null 2>&1

clean:
	@$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null 2>&1
	@printf '$(RED)clean done$(RST)\n'

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	@printf '$(RED)fclean done$(RST)\n'

re: fclean all

.PHONY: all clean fclean re libft mlx