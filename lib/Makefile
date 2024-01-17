NAME = lib42.a
FT_PRINTF = $(addprefix src/ft_, len_nb_hexa.c len_nb.c memset.c pointeur.c printf.c putchar.c \
putnbr_base_hex.c putnbr_base_neg.c putnbr_base.c putstr.c strjoin2.c strncmp.c putstr_fd.c strchr.c atoll.c)
GNL = $(addprefix src/, get_next_line.c get_next_line_utils.c)
OBJS_FT = ${FT_PRINTF:.c=.o}
OBJS_GNL = ${GNL:.c=.o}
OBJ = $(OBJS_FT) $(OBJS_GNL)
CC = gcc
FLAGS = -Wall -Werror -Wextra -g

all: $(NAME)

.c.o:
	$(CC) $(FLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)
	@clear

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
