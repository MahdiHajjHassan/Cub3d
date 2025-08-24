NAME := cub3d

SRC_DIR := src
INC_DIR := include
LIBFT_DIR := $(INC_DIR)/libft
MLX_DIR := minilibx-linux

LIBFT := $(LIBFT_DIR)/libft.a
MLX := $(MLX_DIR)/libmlx.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parser/parse.c \
	$(SRC_DIR)/parser/parse_utils.c \
	$(SRC_DIR)/render/run_game.c \
	$(SRC_DIR)/render/engine.c \
	$(SRC_DIR)/render/input.c \
	$(SRC_DIR)/render/textures.c \
	$(SRC_DIR)/utils/file_utils.c \
	$(SRC_DIR)/utils/str_utils.c \
	$(SRC_DIR)/utils/error.c

SRCS_BONUS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parser/parse_bonus.c \
	$(SRC_DIR)/render/run_game_bonus.c \
	$(SRC_DIR)/render/engine_bonus.c \
	$(SRC_DIR)/render/input_bonus.c \
	$(SRC_DIR)/render/textures_bonus.c \
	$(SRC_DIR)/render/sprites_bonus.c \
	$(SRC_DIR)/render/minimap_bonus.c \
	$(SRC_DIR)/utils/file_utils.c \
	$(SRC_DIR)/utils/str_utils.c \
	$(SRC_DIR)/utils/error.c

OBJS := $(SRCS:.c=.o)
OBJS_BONUS := $(SRCS_BONUS:.c=.o)

CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)
LDLIBS := $(LIBFT) $(MLX) -lXext -lX11 -lm

.PHONY: all clean fclean re libft mlx bonus

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

bonus: CFLAGS += -DBONUS
bonus: $(LIBFT) $(MLX) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LDLIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libft: $(LIBFT)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

mlx: $(MLX)
$(MLX):
	-$(MAKE) -C $(MLX_DIR)

clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean || true

re: fclean all
