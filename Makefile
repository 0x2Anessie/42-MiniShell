# Minishell Makefile with separate directory for object files

NAME = minishell
SRC_DIR = srcs
OBJ_DIR = objs

# Définition des sources par répertoire
SRC_LEXER = 		$(addprefix lexer/, lex.c lexer_structure_construction.c \
						lexer_utils.c token_type_assignment.c)
SRC_PARSER = 		$(addprefix parser/, parser2.c parser4.c parser.c \
						parser_utils.c rm_para_quote.c)
SRC_INIT_ENV = 		$(addprefix init_env/, init_env_list.c init_env.c)
SRC_SIGNALS = 		$(addprefix signals/, ctrl_c_signals.c error_signals.c \
						signal_exits.c)
SRC_BUILT_IN = 		$(addprefix built_in/, built_in_utils_2.c \
						built_in_utils.c cd2.c cd3.c cd.c echo2.c echo.c \
						echo_split.c env.c env_create.c env_things.c export.c \
						export_checking.c export_create.c export_parsing_2.c \
						export_parsing.c export_var_exist.c ft_exit.c \
						path_cd.c pwd.c write_echo.c)
SRC_EXEC = $(addprefix exec/, child_process_management_condtion.c \
				child_process_management.c \
				close_pipes_and_fds.c \
				cmd_exec_condition.c \
				cmd_path_resltion_condtion.c \
				command_analysis.c \
				command_execution_condtion.c \
				command_execution.c \
				command_path_resolution_condition.c \
				command_path_resolution.c \
				config_stdin_stdout_commands.c \
				pipe_cmd_management_and_init_vars_condition.c \
				pipe_cmd_management_and_init_vars.c)
SRC_EXPANDER = $(addprefix expander/, \
					char_validation_utils.c \
					dual_quote_status_utils.c \
					env_var_expansion_length.c \
					env_var_expansion_length_condtion.c \
					expand_utils.c \
					expansion_processing_condtion.c \
					expansion_processing_utils.c \
					expansion_processing.c \
					expansion_search_utils.c \
					expansion_validation_utils.c \
					lexeme_expansion_and_manipulation_condition.c \
					lexeme_expansion_and_manipulation_utils.c \
					lexeme_expansion_and_manipulation.c \
					malloc_expand_condtion.c \
					malloc_expand.c \
					quot_state_validations.c \
					quote_management_and_expander_utils.c \
					quote_state_checks.c \
					shell_variable_handling_env_condition.c \
					shell_word_expansion_conditions.c \
					shell_word_expansion.c \
					singular_quote_status_utils.c \
					special_char_handling.c \
					special_character_checks.c \
					split_word_by_quotes.c)
SRC_FREE_ALL = 		$(addprefix free_all/, free1.c)
SRC_INIT_EXEC = 	$(addprefix init_exec/, file_flags.c here_doc_condition.c \
						here_doc.c init_exec_utils_condition.c \
						init_exec_utils.c init_exec.c \
						init_export_condition.c \
						init_export.c init_fd_condition_pipe.c \
						init_fd_condition.c \
						init_fd_heredoc_condition.c \
						init_fd_redirect_condition.c \
						init_fd_word_condition.c init_fd.c)

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
LIBFT = ./lib/lib42.a

# Directory creation command
DIR_DUP = mkdir -p $(@D)

# Nombre total de fichiers
TOTAL_FILES := $(words $(SRCS))

# Compteur de fichiers actuellement compilés
CURRENT_FILE := 0

# Main targets
all: 			$(NAME)
				@echo "               _____   __        __        __            __   __    ";
				@echo "              /     \ |__| ____ |__| _____|  |__   ____ |  | |  |   ";
				@echo "             /  \ /  \|  |/    \|  |/  ___/  |  \_/ __ \|  | |  |   ";
				@echo "            /    Y    \  |   |  \  |\___ \|   Y  \  ___/|  |_|  |__ ";
				@echo "            \____|__  /__|___|  /__/____  >___|  /\_____>____/____/ ";
				@echo "                    \/        \/        \/     \/                   ";
				@echo "                                                                    ";

$(NAME): $(OBJS)
	@cd lib && make > /dev/null
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
	@echo "                                                                      ";
	@echo "                                                                      ";
	@echo "             /\                __              __                          ";
	@echo "       ____  )/ ____   _______/  |_      ____ |  |   ____  _____    ____   ";
	@echo "     /  ___\  /  __ \ /  ___/\   __\   /  ___\|  | /  __ \ \__  \  /    \  ";
	@echo "     \  \___  \  ___/ \___ \  |  |     \  \___|  |_\  ___/ / __  \|   |  \ ";
	@echo "      \____ >  \____ >_____ > |__|      \____ >____/\____> ____  / ___|  / ";
	@echo "                                                               \/      \/  ";
	@echo "                                                                      ";

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
	@echo "                                                                            ";
	@echo "                                                                            ";
	@echo "          /\                __        _____       __                          ";
	@echo "    ____  )/ ____   _______/  |_    _/ ____\____ |  |   ____  _____    ____   ";
	@echo "  /  ___\  /  __ \ /  ___/\   __\   \  __\/  ___\|  | /  __ \ \__  \  /    \  ";
	@echo "  \  \___  \  ___/ \___ \  |  |      |  | \  \___|  |_\  ___/  / __ \|   |  \ ";
	@echo "   \____ >  \____> _____ > |__|      |__|  \____ >____/\____ > ____  /___|  / ";
	@echo "                                                                   \/     \/  ";
	@echo "                                                                      ";

re: fclean all

.PHONY: all clean fclean re
