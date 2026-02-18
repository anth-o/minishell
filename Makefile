NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# --- CONFIGURATION LIBFT ---
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
# ---------------------------

INC = includes
SRC = src/main.c \
	src/signals.c \
	src/parsing/env_init.c \
	src/parsing/argv_to_token.c \
	src/parsing/token_to_cmd.c \
	src/parsing/handle_vars.c \
	src/parsing/check_syntax.c \
	src/parsing/env_utils.c \
	src/exec/path_utils.c \
	src/exec/exec_simple.c \
	src/exec/redir.c \
	src/builtin/builtin_utils.c \
	src/builtin/ft_cd.c \
	src/builtin/ft_env.c \
	src/builtin/ft_exit.c \
	src/builtin/ft_pwd.c \
	src/builtin/ft_echo.c \
	src/builtin/ft_export.c \
	src/builtin/ft_unset.c

OBJ = $(SRC:.c=.o)

LIBS = -lreadline

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_DIR) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -I $(INC) -I $(LIBFT_DIR) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re