NAME = minishell

CC = cc
RM = rm -f
CFLAGS = -g -g3 -Wall -Wextra -Werror

SRCS = $(addprefix Src/, main.c lexer/lexer_utils.c lexer/lex.c lexer/lexer.c parser/parser.c parser/parser2.c parser/rm_para_quote.c \
					parser/parser_utils.c init_env/init_env.c signals/error_signals.c init_exec/init_exec.c init_exec/init_exec_utils.c \
					init_exec/init_export.c init_exec/init_fd.c exec/close_fd.c signals/ctrl_c_signals.c\
					built_in/pwd.c built_in/echo.c built_in/echo_utils.c built_in/cd.c built_in/built_in_utils.c built_in/env.c\
					built_in/env_create.c built_in/export_parsing.c built_in/export_var_exist.c built_in/export.c built_in/export_checking.c\
					init_exec/here_doc.c exec/child.c exec/exec_utils.c exec/prep_exec.c exec/redir_child.c \
					expander/expand2.c built_in/ft_exit.c free_all/free1.c expander/expand_utils.c built_in/built_in_utils_2.c \
					built_in/export_create.c built_in/export_parsing_2.c built_in/env_things.c expander/malloc_expand.c \
					expander/quoteandnode.c expander/expand.c expander/expand_len.c expander/splitandquote.c expander/state_quote.c parser/parser4.c \
					built_in/echo2.c init_env/init_env2.c built_in/cd2.c built_in/cd3.c built_in/write_echo.c)	
OBJ = $(SRCS:.c=.o)

all: 			$(NAME)
				@clear
				@echo "   _____   __        __        __            __   __    ";
				@echo "  /     \ |__| ____ |__| _____|  |__   ____ |  | |  |   ";
				@echo " /  \ /  \|  |/    \|  |/  ___/  |  \_/ __ \|  | |  |   ";
				@echo "/    Y    \  |   |  \  |\___ \|   Y  \  ___/|  |_|  |__ ";
				@echo "\____|__  /__|___|  /__/____  >___|  /\_____>____/____/ ";
				@echo "        \/        \/        \/     \/                   ";

$(NAME): 		$(OBJ)
				cd Lib && make
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ) Lib/lib42.a -I ./Lib/ -lreadline

bonus: 			$(OBJ_BONUS)
				cd Lib && make
				$(CC) $(CFLAGS) -o $(NAME2) $(OBJ_BONUS) Lib/lib42.a -I ./Lib/ 

leak:
				valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=rl_leaks.txt ./$(NAME)

clean:
				cd Lib && make clean
				$(RM) $(OBJ)
				$(RM) $(OBJ_BONUS)
				@clear
				@echo "        /\                __              __                          ";
				@echo "  ____  )/ ____   _______/  |_      ____ |  |   ____  _____    ____   ";
				@echo "/  ___\  /  __ \ /  ___/\   __\   /  ___\|  | /  __ \ \__  \  /    \  ";
				@echo "\  \___  \  ___/ \___ \  |  |     \  \___|  |_\  ___/ / __  \|   |  \ ";
				@echo " \____ >  \____ >_____ > |__|      \____ >____/\____> ____  / ___|  / ";
				@echo "                                                          \/      \/  ";

fclean:			clean
				$(RM) $(NAME)
				@clear
				@echo "        /\                __        _____       __                         ";
				@echo "  ____  )/ ____   _______/  |_    _/ ____\____ |  |   ____  _____    ____   ";
				@echo "/  ___\  /  __ \ /  ___/\   __\   \  __\/  ___\|  | /  __ \ \__  \  /    \  ";
				@echo "\  \___  \  ___/ \___ \  |  |      |  | \  \___|  |_\  ___/  / __ \|   |  \ ";
				@echo " \____ >  \____> _____ > |__|      |__|  \____ >____/\____ > ____  /___|  / ";
				@echo "                                                                 \/     \/  ";

re:				fclean all

.PHONY:			all clean fclean re
