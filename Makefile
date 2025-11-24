NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INC = includes
SRC = src/main.c \
	src/signals.c \
	src/parsing/argv_to_token.c \
	src/parsing/token_to_cmd.c \
	src/parsing/handle_vars.c

OBJ = $(SRC:.c=.o)

LIBS = -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -I $(INC) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
