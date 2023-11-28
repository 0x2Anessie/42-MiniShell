# Minishell Makefile with separate directory for object files

NAME = minishell
SRC_DIR = src
OBJ_DIR = objs

# Définition des sources par répertoire
SRC_LEXER = $(addprefix lexer/, lexer_utils.c lex.c lexer.c)
SRC_PARSER = $(addprefix parser/, parser2.c parser4.c parser.c parser_utils.c \
        		rm_para_quote.c)
SRC_INIT_ENV = $(addprefix init_env/, init_env2.c init_env.c)
SRC_SIGNALS = $(addprefix signals/, signals2.c signals.c)
SRC_BUILT_IN = $(addprefix built_in/, built_in_utils_2.c built_in_utils.c \
				cd2.c cd3.c cd.c echo2.c echo.c echo_utils.c env.c \
				env_create.c env_things.c export.c export_checking.c \
				export_create.c export_parsing_2.c export_parsing.c \
				export_var_exist.c ft_exit.c path_cd.c pwd.c write_echo.c)
SRC_EXEC = $(addprefix exec/, child.c close_fd.c command_analysis.c \
			command_execution.c exec_utils.c redir_child.c)
SRC_EXPANDER = $(addprefix expander/, shell_variable_handling.c shell_word_expansion.c expand_len.c \
					    expand_utils.c malloc_expand.c quoteandnode.c \
						splitandquote.c state_quote.c)
SRC_FREE_ALL = $(addprefix free_all/, free1.c)
SRC_INIT_EXEC = $(addprefix init_exec/, here_doc.c init_exec.c \
					init_exec_utils.c init_export.c init_fd.c)
					
# Combinaison de tous les fichiers sources
SRCS = main.c \
       $(SRC_LEXER) \
       $(SRC_PARSER) \
       $(SRC_INIT_ENV) \
       $(SRC_SIGNALS) \
       $(SRC_BUILT_IN) \
       $(SRC_EXEC) \
       $(SRC_EXPANDER) \
       $(SRC_FREE_ALL) \
       $(SRC_INIT_EXEC)

SRCS := $(SRCS:%=$(SRC_DIR)/%)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD
LIBFT = ./Lib/lib42.a

# Directory creation command
DIR_DUP = mkdir -p $(@D)

# Nombre total de fichiers
TOTAL_FILES := $(words $(SRCS))

# Compteur de fichiers actuellement compilés
CURRENT_FILE := 0

# Main targets
all: $(NAME)

$(NAME): $(OBJS)
	@cd Lib && make > /dev/null
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo ""

# Fonction pour mettre à jour et afficher la barre de progression sur une seule ligne
define update_progress
    $(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
    $(eval PERCENT=$(shell echo $$((($(CURRENT_FILE)*100)/$(TOTAL_FILES)))))
    @printf "\r  make minishell : ["
    @printf "%-50s" "$$(printf '#%.0s' $$(seq 1 $$(($(PERCENT)/2))))"
    @printf "] %d%%" $(PERCENT)
endef

# Rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(DIR_DUP)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(call update_progress)

# Include dependencies
-include $(DEPS)

# Function to update and display the progress bar on a single line
define clean_progress
	$(eval TOTAL_COUNT=$(words $(OBJS) $(DEPS) 1)) # +1 for the objs directory
	$(eval CURRENT_COUNT=$(shell echo $$(($(CURRENT_COUNT)+1))))
	$(eval PERCENT=$(shell echo $$((($(CURRENT_COUNT)*100)/$(TOTAL_COUNT)))))
	@printf "\r clean minishell : ["
	@printf "%-50s" "$$(printf '#%.0s' $$(seq 1 $$(($(PERCENT)/2))))"
	@printf "] %d%%" $(PERCENT)
endef

# Other rules (clean, fclean, re)
clean:
	$(eval CURRENT_COUNT=0)
	@$(foreach obj,$(OBJS) $(DEPS),rm -f $(obj); $(call clean_progress);)
	@rm -rf $(OBJ_DIR)
	@$(call clean_progress) # This will now correctly show 100%
	@echo ""

# Fonction pour mettre à jour et afficher la barre de progression sur une seule ligne pour fclean
define fclean_progress
	$(eval TOTAL_COUNT=1) # Only for the minishell executable
	$(eval CURRENT_COUNT=$(shell echo $$(($(CURRENT_COUNT)+1))))
	$(eval PERCENT=$(shell echo $$((($(CURRENT_COUNT)*100)/$(TOTAL_COUNT)))))
	@printf "\rfclean minishell : ["
	@printf "%-50s" "$$(printf '#%.0s' $$(seq 1 $$(($(PERCENT)/2))))"
	@printf "] %d%%" $(PERCENT)
endef

# Other rules (clean, fclean, re)
fclean:
	$(eval CURRENT_COUNT=0)
	@$(MAKE) clean > /dev/null
	@rm -f $(NAME)
	@$(call fclean_progress)
	@echo ""

re: fclean all

.PHONY: all clean fclean re