#ifndef MINISHELL_H
# define MINISHELL_H

/*   LIBRARY   */
# include <readline/readline.h>
# include <stdio.h>
# include <readline/history.h>
# include "../lib/includes/lib.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdbool.h>

/*   STATUS AND FLAG DEFINITIONS   */
# define FAIL -1
# define TRUE 1
# define FALSE 0
# define ZERO_INIT 0
# define EXIT_SUCCESS 0
# define FT_FAILURE_EXIT 0
# define CHILD_PROCESS 0
# define FORK_FAILURE -1
# define SKIP_DOLLAR_QUESTION_MARK 2
# define IDX_ADJUST -1
# define EXIT_FAIL 1
# define ERR_CODE_CMD_NOT_FOUND 127
# define CHAR_IS_SPECIAL 1
# define CHAR_IS_NOT_SPECIAL 0
# define CHAR_IS_DOLLAR 0
# define CHAR_IS_NOT_DOLLAR 1
# define NEED_EXPAND 1
# define NO_EXPAND 0
# define EXPANSION_REQUIRED 1
# define NO_ACTION_REQUIRED 0
# define INVALID_PIPE -2

/*   OFFSET   */
# define FORWARD_OFFSET 1
# define BACKWARD_OFFSET -1

/*   OUTPUT REDIRECTION ERROR ID   */
# define OUTPUT_TARGET_ACCESS_ERROR_CODE 1
# define OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE 2

/*   INPUT_FD_VALUE   */
# define INPUT_FD_NOT_SET -1
# define INPUT_FD_REDIRECTION_FAIL -2

/*   OUTPUT_FD_VALUE   */
# define OUTPUT_FD_NOT_CONFIGURED -1

/*   GENERAL   */
# define NOT_FOUND 0
# define FOUND 1

/*   CMD   */
# define CMD_NOT_FOUND 0
# define CMD_FOUND 1

/*   SIGNAL   */
# define SIGNAL_EXIT_OFFSET 128
# define CTRL_C_SIGINT SIGINT
# define CTRL_BACKSLSH SIGQUIT
# define IGNORE_SIG_ACTION SIG_IGN

/*   FILE AND STREAM CONSTANTS   */
# define HEREDOC_TEMP_FILE "here_doc.tmp"
# define OUT_FILE "Outfile"

/*   PERMISSION   */
# define PERM_RWX_ALL 0777
# define PERM_O_RW_G_R_OT_R 0644

/*   ERROR MESSAGES AND SIGNALS   */
# define ERR_HEREDOC_EOF_WARNING "minishell: warning: here-document delimited \
by end-of-file (wanted `EOF')\n"
# define ERR_WRIT_NO_SPAC_LEFT_DEVC "write error: no space left on device\n"
# define ERR_MSG_CMD_NOT_FOUND ": command not found\n"
# define ERR_AMB_REDIRECT "minishell: ambiguous redirect\n"
# define ERR_MEMORY_ALLOCATION "Erreur lors de l'allocation de mémoire"
# define WRITE_ERROR_MSG "write error"
# define QUIT_MESSAGE "Quit\n"

# define ERR_MEMORY_ALLOCATION_FAILURE "Échec de l'allocation de mémoire pour \
value_of_expanded_var_from_env\n"

/*   PIPE AND PROCESS CONSTANTS   */
# define PIPE_READ_END 0
# define PIPE_WRITE_END 1

/*   SHELL COMMAND AND TOKEN CONSTANTS   */
# define TOKEN_IS_COMMAND 1
# define TOKEN_IS_PIPE 0
# define INFINITY_LOOP 1

/*   STRING AND CHARACTER CONSTANTS   */
# define FT_NEWLINE "\n"
# define TAB '\t'	 // Tabulation horizontale
# define SHIFT_OUT 14 // SO (Shift Out)
# define IS_SEPARATOR 1
# define NOT_SEPARATOR 0
# define SLASH_FOUND 0
# define NO_SLASH_FOUND 1

/*   Single and double quote  */
# define SINGLE_QUOTE_NOT_STARTED 0
# define SINGLE_QUOTE_STARTED 1
# define SIMPL_QUOT_CLOS 0
# define SIMPL_QUOT_OPEN 1
# define DOUBL_QUOT_CLOS 0
# define DOUBL_QUOT_OPEN 1
# define QUOTED 1
# define NOT_QUOTED 0

/*   DEFINE CMD BUILING    */
# define CMD_ECHO "echo"
# define CMD_ENV_VARS "env"
# define CMD_EXIT_SHELL "exit"
# define CMD_UNSET_VARS "unset"
# define CMD_EXPORT_VARS "export"
# define CMD_CHANG_DIRCT "cd"
# define CMD_PRINT_DIRCT "pwd"
# define CMD_CHANGE_DIRECTORY "chdir"
# define CMD_UNSET_ENV_VAR "unset"

/*   ENVIRONMENT VARIABLE SET   */
# define ENV_SET_CURRENT_WORKING_DIR "PWD="
# define ENV_SET_USER_HOME_DIR "HOME="
# define ENV_SET_OLDPWD "OLDPWD="
# define ENV_SET_PATH_PREFIX "PATH="

/*   ENVIRONMENT VARIABLE NAMES   */
# define ENV_PREVIOUS_WORKING_DIR "OLDPDW"
# define ENV_CURRENT_WORKING_DIR "PWD"
# define ENV_USER_HOME_DIR "HOME"

typedef enum s_token
{
	CMD,
	ARG,
	REDIRECT_OUT,
	APPEND_OUT,
	REDIRECT_IN,
	HERE_DOC,
	REDIRECT_INOUT,
	REDIRECT_OUTIN,
	FD,
	DELIMITER,
	PIPE,
}	t_token;

typedef struct s_node
{
	int				input_fd;
	int				output_fd;
	int				here_doc_fd;
	int				is_command_present;
	int				is_output_redirection_feasible;
	int				is_input_redirection_failed;
	int				output_redirection_error_id;
	char			*heredoc_tmp_fullname;
	struct s_node	*next;
}	t_node;

typedef struct s_env
{
	char			*var_env_name_and_value;
	struct s_env	*next_var_env_name_and_value;
}	t_env;

typedef struct s_stringdata
{
	int				indx_quote_escape_handling;
	int				indx_rebuilt_str_excl_quotes;
	int				word_len_of_cmd_with_quot_includ;
	int				is_current_char_preceded_by_escape_char;
}	t_stringdata;

typedef struct s_lexer
{
	int				segment_position;
	char			*cmd_segment;
	t_token			token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_quote
{
	int				singl_quot_status;
	int				doubl_quot_status;
	int				singl_quot_start_status;
	int				var_env_match_confirmed;
}	t_quote;

typedef struct s_expand
{
	int				length_of_expanded_var_value;
	int				var_env_match_found;
	int				dollar_sign_present;
	int				sing_or_doub_quote_in_env_expansion;
	char			*value_of_expanded_var_from_env;
}	t_expand;

typedef struct s_export
{
	char			*env_var_name_and_value;
	struct s_export	*next_env_var_name_and_value;
}	t_export;

typedef struct s_exec
{
	int				heredoc_ctrl_c_uninterrupted;
	int				heredoc_nbr;
	int				stdin_fd_for_heredoc;
	int				is_this_an_exec_in_heredoc;
	int				nb_cmd_in_lst;
	int				cmd_nb_pipe;
	int				previous_fd;
	char			**full_env_var_copy_beta;
	char			*heredoc_input_buffer;
	char			*g_signal_in_char_format;
	char			*name_of_var_env_to_del;
	t_env			*linked_list_full_env_var_copy_alpha;
	t_env			*head_env_lst;
	t_node			*head_cmd_lst;
	t_lexer			*head_lexer_lst;
	t_export		*head_of_linked_list_env_var;
	struct s_node	*node;
	struct s_exec	*next;
	struct s_exec	*prev;
}	t_exec;

typedef struct s_trash_memory
{
	void					*add;
	struct s_trash_memory	*next;
}	t_trash_memory;

typedef struct s_data
{
	int				env_var_line_idx;
	int				is_sing_quot;
	int				is_doub_quot;
	int				command_line_arg_count;
	int				input_line_word_count;
	char			*minishell_input_cmd;
	char			**command_line_args;
	char			**full_env_var_copy_alpha;
	char			**full_env_var_copy_gamma;
	t_exec			*utils;
	t_quote			*quote;
	t_expand		*expand;
	t_lexer			*lexer_list;
	t_trash_memory	*trash_memory;

}	t_data;

extern unsigned int	g_globi;

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                  MAIN                                     */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de main.c   */
void		prompt_loop(char *tmp, t_data *data, char **env);

/*   Fonctions de main_utils.c   */
int			is_operator(char c);
int			is_double_operator(const char *command, size_t pos);
size_t		calculate_new_length(const char *command);
void		fill_command_with_spaces(const char *command, char *new_command, \
			t_quote *state, size_t i);
char		*add_spaces_around_operators(t_data *data, const char *command);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                BUILT_IN                                   */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de built_in_utils_2.c   */
int			ft_string_length(char *s);
char		*ft_strjoin_with_memory_tracking(char *s1, char *s2, t_data *data);
char		*ft_strjoin_free_arg2_with_memory_tracking( \
			char *s1, char *s2, t_data *data);
size_t		ft_str_len_until_equal(char *str);

/*   Fonctions de built_in_utils.c   */
char		*ft_strcpy(char *dest, const char *src);
char		*add_quotes_string(char *str, t_data *data);
char		*join_them(char *str, int i, int flag, t_data *data);
char		*case_egale(char *str, t_data *data);
void		exit_all(t_data *data);

/*   Fonctions de cd.c   */
int			verif_pwd(char *str, t_data *data);
int			verif_home(char *str, t_data *data);
int			change_directory_for_home(t_env *tmp, t_data *data);
char		*get_home(t_env *tmp, t_data *data);

/*   Fonctions de cd2.c   */
int			change_directory_for_pwd(t_env *tmp, t_data *data);
int			wrong_cd(t_lexer *lexer_lst);
int			cd_with_arg(t_data *data, char *path, char *old, int *i);
char		*create_new_var(char *str, t_data *data);
void		get_cd(t_lexer *lexer_lst, t_data *data);

/*   Fonctions de cd3.c   */
int			change_directory(char *dir);
int			change_directory_for_oldpwd(t_env *tmp, t_data *data);
int			verif_oldpwd(char *str, t_data *data);
char		*get_pwd_env(t_env *tmp, t_data *data);

/*   Fonctions de echo_utils.c   */
int			ft_word_count(char *s, char c);
char		*ft_put_word_in_malloc(char *s, char c, char *tab);
char		**ft_malloc_word(char **tab, char *s, char c, t_data *data);
char		**ft_split_mini(char const *s, char c, t_data *data);
void		freetab(char **tab, int k);

/*   Fonctions de echo.c   */
int			ft_strcmp(char *s1, char *s2);
void		display_echo_arg(char **tab, int i, t_data *data);
void		get_words(t_lexer *lexer_lst, char **tab, int *i);
void		process_echo(char **tab, int i, t_data *data);
void		procc_in_echo_std(char **tab, int i, t_data *data);

/*   Fonctions de echo2.c   */
int			check_n(char *str);
void		init_echo(t_lexer *lexer_lst, t_data *data);
void		simulate_echo(char **tab, t_data *data);

/*   Fonctions de env_create.c   */
void		lst_add_back(t_exec *utils, char *str, t_data *data);
t_env		*create_node_str(char *str, t_data *data);

/*   Fonctions de env_things.c   */
void		remove_node(char *var, t_data *data);
void		remove_env_node(t_env *current, t_env *prev, t_data *data);
void		remove_node_export(char *var, t_data *data);
void		remove_export_node( \
			t_export *current, t_export *prev, t_data *data);

/*   Fonctions de env.c   */
int			check_env(t_lexer *lexer_lst, t_data *data);
int			get_unset(t_lexer *lexer_lst, t_data *data);
char		*check_unset(t_lexer *lexer_lst);
void		display_env(t_data *data);

/*   Fonctions de export_checking.c   */
int			check_case_with_char(char *str);
int			check_case(char *str);
void		export_quotes(t_export *don, t_data *data);
void		supp_quote_and_add_env(t_exec **utils, t_lexer *tmp, t_data *data);

/*   Fonctions de export_create.c   */
void		lst_add_back_export(t_export **head, char *str, t_data *data);
t_export	*create_node_str_export(char *str, t_data *data);

/*   Fonctions de export_parsing_2.c   */
int			is_number(char c);
int			check_parsing_char(char *str, int i);
int			check_parsing_error(char *str);
int			check_parsing_export(char *str);

/*   Fonctions de export_parsing.c   */
void		initialize_data(t_stringdata *data, char *str);
void		extract_within_quotes(t_stringdata *data, char *str);
void		extract_quotes(t_stringdata *data, char *str, char quote);
void		remove_single_quotes(char *str);
void		remove_double_quotes(char *str);

/*   Fonctions de export_var_exist.c   */
int			verif_var_exist_export(t_exec *utils, char *str, t_data *data);
int			verif_equal(char *str, char c);
int			verif_var_exist(t_exec *utils, char *str);
int			verif_var_exist_export_not_maj(t_exec *utils, char *str);

/*   Fonctions de export.c   */
int			export_parsing_syntaxe(t_lexer *tmp, t_data *data);
int			export_things(t_lexer *lexer_lst, t_data *data);
void		print_export(t_export *head_of_linked_list_env_var, t_data *data);
void		process_word_and_add_export(t_lexer *tmp, t_data *data);
void		export_remaining(t_lexer *tmp, t_data *data);

/*   Fonctions de ft_exit.c   */
int			is_valid_number2(int neg, char *str, int i);
int			is_valid_number(char *str);
void		free_res(int *fd, t_data *data);
void		ft_exit_with_code( \
			t_lexer *lex, int *fd, unsigned char exit_code, t_data *data);
void		ft_exit(t_lexer *lex, int *fd, t_data *data);

/*   Fonctions de path_cd.c   */
int			verif_oldpwd_export(char *str, t_data *data);
int			verif_pwd_export(char *str, t_data *data);

/*   Fonctions de pwd.c   */
int			get_pwd(char *tab, t_data *data);
void		display_pwd(t_data *data);
void		display_pwd_error(t_data *data);
void		find_old_pwd(t_env *env, t_data *data);

/*   Fonctions de write_echo.c   */
int			procc_in_echo_fd(char **tab, int i, int j, t_data *data);
void		write_echo_fd(char **tab, int i, t_data *data);

/*   Fonctions divers de built_in   */
char		*ft_strjoin2_mini(char *s1, char *s2, t_data *data);
char		*get_old_pwd(t_env *tmp, t_data *data);
void		execute_lexer_command_with_args(t_data *data);
void		ft_putstr_fd_mini(char *s1, char *s2, int fd, t_data *data);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                  EXEC                                     */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de child_process_management_condtion.c   */
bool		is_execve_failed(char *command_path, char **arguments, char **env);

/*   Fonctions de child_process_management.c   */
int			check_for_slash_path_delimiter(t_lexer *lexer);
void		execute_lexer_command_with_args(t_data *data);
void		exec_cmd_with_redirection_and_builtins(\
			t_data	*data, int *fd, int count, t_exec utils);
pid_t		create_and_execute_child_process(\
			t_data	*data, int *fd, int count, t_exec utils);

/*   Fonctions de close_pipes_and_fds.c   */
int			closes_ends_of_pipe(int *fd);
int			close_file_descriptors_in_linked_list(t_data *data);

/*   Fonctions de cmd_exec_condition.c   */
bool		is_pipe_creation_failed(int *fd, int total_number_of_cmd);
bool		is_single_command_in_list(t_data *data);
bool		is_single_builtin_command(t_data *data);

/*   Fonctions de cmd_path_resltion_condtion.c   */
bool		is_env_var_path_name(t_env *env);
bool		is_invalid_direct_path(const char *cmd);

/*   Fonctions de command_analysis.c   */
int			is_cmd_match( \
			t_lexer *lexer_lst, const char *command, int command_length);
int			is_built_in_command(t_lexer *lexer_lst);
int			should_continue_execution(t_data *data, int *y);
int			check_redirection_validity_in_node(t_node *node);
t_lexer		*find_next_command_in_lexer(t_lexer *lexer_list);

/*   Fonctions de command_execution_condtion.c   */
bool		is_pid_array_null(pid_t *pid);
bool		is_process_pid_valid_for_wait(pid_t pid);
bool		is_child_process_exited_cleanly(int status);
bool		should_continue_waiting_for_child_processes(\
int nb_node, t_data *data);

/*   Fonctions de command_execution.c   */
int			manage_exec_linked_cmd_sequence(int *fd, pid_t *pid, \
			t_data *data, int *y);
void		ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data);
void		close_fds_if_needed(int *fd, int previous_fd);
void		wait_and_process_child_statuses(\
			pid_t *pid, int *wstatus, int nb_node, t_data *data);
void		manage_execution_resources(t_data *data);

/*   Fonctions de command_path_resolution_condition.c   */
bool		is_invalid_direct_path(const char *cmd);

/*   Fonctions de command_path_resolution.c   */
int			check_if_cmd_full_path_exec_is_valid(t_lexer *lexer, \
			t_exec utils, t_data *data);
char		**extract_paths_from_env(t_env *env_lst, t_data *data);
char		*find_command_full_path(char *cmd, t_env *env_lst, t_data *data);
char		**build_arg_array_from_lexer(t_data *data);

/*   Fonctions de config_stdin_stdout_commands.c   */
bool		is_fd_valid_for_close(int fd);
bool		is_fd_set_for_redirection(int fd);
void		configure_stdin_redirection_for_command(t_exec utils, int count);
bool		is_not_last_command_and_fd_open(int count, int total_cmds, int fd);
void		configure_stdout_redirection_for_command( \
			t_exec utils, int *fd, int count);

/*   Fonctions de pipe_cmd_management_and_init_vars.c  */
int			count_args_until_pipe_for_cmd_array(t_lexer *lexer_list);
void		initialize_pid_array_to_zero(pid_t *pid_array, int num_nodes);
void		init_fd_and_status_vars(int *fd, int *y, int *wstatus);
t_lexer		*reaches_next_cmd_preceded_by_pipe(t_lexer *lexer_list);

/*   Fonctions de pipe_cmd_management_and_init_vars_condition.c   */
bool		is_current_token_cmd_arg(t_lexer *lexer_lst);
bool		is_index_less_than_num_nodes(int index, int num_nodes);

/*   Fonctions de command_path_resolution_condition.c   */
bool		is_cmd_empty_or_null(char *cmd);
bool		is_cmd_executable(char *command);
bool		is_lexer_token_cmd_arg(t_lexer *lexer_item);
bool		should_continue_lexer_iteration( \
			t_lexer	*lexer_item, int current_index, int total_args);
bool		is_env_list_empty_or_null(t_env *env_lst);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                EXPANDER                                   */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*     Fonctions de char_validation_utils.c    */
bool		is_next_char_valid_and_not_single_quote(const char *word, \
			int *indx);
bool		is_not_end_of_string_or_dollar_sign(char char_to_check);
bool		is_current_char_question_mark(const char *word, int index);
bool		is_error_code_char_present(t_data *data, int indx);

/*     Fonctions de dual_quote_status_utils.c     */
int			is_both_quotes_off(t_quote *state);
int			is_only_single_quote_on(t_quote *state);
int			is_only_double_quote_on(t_quote *state);
int			is_both_quotes_on(t_quote *state);

/*     Fonctions de env_var_expansion_length.c     */
int			get_env_var_expansion_length(\
			char *word, int *length, char *env_var, t_quote *state);
int			calculate_length_for_env_expansion(char *word, \
			char **env_var, t_quote *state, int *length);
int			calculate_expanded_env_var_length(\
			char *word, char **env_var, t_quote *state, int *length);
int			calculate_length_until_single_quote(char *word, int *length);
bool		is_char_matching_env_var(\
			char *word, int word_index, char *env_var, int env_var_index);

/*     Fonctions de expand_utils.c     */
int			length_of_number_for_string_conversion(long n);
int			is_decimal_digit(char c);
char		*convert_negative_int_to_string_with_tracking( \
			t_data *data, char *res, long n, int length);
char		*convert_int_to_string_with_tracking(t_data *data, int n);
void		reset_quoting_state(t_quote *state);

/*     Fonctions de expansion_processing_condtion.c     */
bool		is_current_char_dollar_sign(t_lexer *exp, int index);
bool		is_next_char_end_space_dash_or_special(\
t_lexer *expnd, t_quote *state, int index);

/*     Fonctions de expansion_processing_utils.c     */
int			append_chars_expnt_until_singl_quot(char *word, t_expand *exp);
int			append_curnt_error_code_to_expansion_struc( \
			t_expand *exp, t_data *data);

int			expand_env_vars_with_question_mark_handling( \
			char *w, t_expand *exp, t_data *data, t_quote *state);

/*     Fonctions de expansion_processing.c    */
bool		is_current_char_dollar_sign(t_lexer *exp, int index);
void		expand_variables_and_handle_special_chars( \
			t_lexer **expnd, t_quote *state, t_expand *exp, t_data *data);
void		length_of_env_variable_value(\
			t_lexer **exp, t_quote *state, t_data *data, int *expanded_length);

/*     Fonctions de expansion_search_utils.c   */
int			find_and_expand_env_var_with_special_char( \
			char *w, t_expand *exp, t_data *data, t_quote *state);
int			handle_unfound_expansion_word(char *w, t_quote *state);

/*     Fonctions de expansion_validation_utils.c   */
bool		is_word_end_or_special(const char *word, int index, t_quote *state);
bool		is_expansion_not_found(t_expand *exp);
bool		is_expansion_found(t_expand *exp);

/*     Fonctions de lexeme_expansion_and_manipulation_utils.c   */
void		reset_quote_states(t_quote *st);
void		initialize_expansion_parameters(\
t_expand *exp, t_data *data, t_lexer **expnd, t_quote *st);

/*     Fonctions de lexeme_expansion_and_manipulation_condition.c   */
bool		is_expansion_required_and_unquoted(t_expand *exp);
bool		is_there_remaining_elements(int index, int total_length);
bool		is_single_quote_with_closed_double_quotes(char c, t_quote *state);
bool		is_double_quote_with_closed_single_quotes(char c, t_quote *state);
bool		is_dollar_char_then_end_of_string(t_lexer *exp);

/*     Fonctions de lexeme_expansion_and_manipulation.c   */
char		*allocate_memory_for_expanded_word(\
			t_lexer **exp, t_quote *st, char **env, t_data *data);
void		expand_and_insert_in_lexeme_linked_list(\
			t_lexer **expnd, t_quote *st, t_data *data, t_expand *exp);
void		insert_additional_lexemes(\
			char **insert, t_lexer *back, t_data *data, int len);
void		replace_and_extend_chained_list_lexeme(\
			t_lexer **old_node, char **to_insert, t_data *data);
char		*copy_string_excluding_quotes(\
			char *original_cmd_with_quotes, \
			char *cmd_without_quotes_ready_for_expand, t_quote *state);

/*     Fonctions de malloc_expand_condtion.c    */
bool		is_char_non_space_and_copyable(char current_char);
bool		is_next_char_non_space(char *word, int index);
bool		is_next_char_present(char *word, int index);
bool		is_start_of_word(char *word, int index);

/*     Fonctions de malloc_expand.c    */
char		*malloc_copy_string_excluding_inactive_quotes(\
			int len, char *old, t_quote *state, t_data *data);
char		*create_cleaned_str_excluding_inactive_quots(\
			char *old, t_quote *state, t_data *data);
int			is_start_word_considering_quotes(\
			char *word, int index, t_quote *state);
char		*copy_alloc_memory_excluding_inactive_quot(\
			char *word, t_quote *state, char *str);
char		*alloc_copy_word_excluding_quot_with_tracking(\
			char *word, t_quote *state, t_data *data);

/*     Fonctions de quot_state_validations.c   */
int			is_single_quote_started(t_quote *state);
int			is_single_quote_started_double_quote_closed(t_quote *state);
int			is_single_quote_open_and_started(t_quote *state);

/*     Fonctions de quote_management_and_expander_utils.c   */
int			is_char_cmd_separator(char c);
int			is_end_of_command_word(char *cmd, int i, t_quote *state);
int			count_string_array_elements(char **arr);
void		update_quoting_state(char c, t_quote *state);
bool		is_cmd_separator_and_quotes_off(char c, t_quote *state);

/*     Fonctions de quote_state_checks.c   */
bool		is_quote_not_part_of_string(char current_char, t_quote *state);
bool		is_single_quote_with_double_quote_closed( \
			char current_char, t_quote *state);
bool		is_double_quote_with_single_quote_closed(\
			char current_char, t_quote *state);
bool		is_quote_char_with_closed_state(char current_char, t_quote *state);

/*     Fonctions de shell_variable_handling_env_condition.c  */
bool		is_matching_env_var_name(\
			char *w, t_data *data, int i, int env_var_char_idx);
bool		is_valid_env_var_entry(t_data *data);
bool		is_env_var_value_non_empty(t_data *data, int env_var_char_idx);
bool		is_env_var_name_with_equal_sign(t_data *data, int env_var_char_idx);

/*     Fonctions de shell_word_expansion_conditions.c   */
bool		is_expansion_not_required_and_quoted(t_expand *exp);
bool		is_singl_or_doubl_quote(char character);
bool		is_dollar_sign(char character);
bool		is_the_token_a_delimiter(t_token token);
bool		is_word_non_empty(const char *word);

/*     Fonctions de shell_word_expansion.c   */
int			determine_expansion_or_quote_removal(\
			t_lexer **to_check, t_quote *state, t_expand *exp, t_data *data);
void		process_lexer_for_expansion(\
			t_quote *state, t_data *data, t_expand *exp, t_lexer *lexeme);
void		expand(t_quote *state, char **env, t_lexer *tmp, t_data *data);

/*     Fonctions de singular_quote_status_utils.c     */
int			is_single_quote_open(t_quote *state);
int			is_single_quote_closed(t_quote *state);
int			is_doubl_quote_open(t_quote *state);
int			is_doubl_quote_closed(t_quote *state);

/*     Fonctions de special_char_handling.c     */
int			is_next_char_end_or_special(char *word, t_quote *state);
int			is_next_char_decimal_digit(char *word);
int			is_special_char_found_with_state_not_found(\
			t_lexer *exp, t_quote *state, int index);
int			is_next_char_question_mark(t_lexer *exp, int index);

/*     Fonctions de special_character_checks.c    */
int			is_special_syntax_character(char c, t_quote *st);
int			is_dollar_or_doubl_or_singl_quote(char *word);
int			is_dollar_at_end(char *str);

/*     Fonctions de split_word_by_quotes.c     */
int			count_subwords_with_quote_states(char *word, t_quote *state);
char		**split_word_by_quotes(t_data *data, char *word, t_quote *state);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                   FREE                                    */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*     Fonctions de free.c    */
int			ft_exit_child(int *fd, t_data *data);
void		add_to_trash_memory(t_data *data, void *add);
void		*malloc_track(t_data *data, size_t size);
void		ft_free(void *add, t_data *date);
void		ft_free_all(t_data *data);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                INIT_ENV                                   */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*     Fonctions de init_env_list.c    */
int			env_size(char **env);
void		ft_env_lst_add_to_end(t_env *lst, t_env *new);
t_env		*create_and_init_env_var_node(t_data *data, char *env);
t_env		*create_env_list_from_array(t_data *data, char **env);

/*     Fonctions de init_env.c   */
void		malloc_no_env_initial_node(t_data *data, char *str, t_env **env);
t_env		*init_env_list_with_pwd_if_empty(t_data *data, t_env *final);
t_exec		*init_env(t_data *data, char **env);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                               INIT_EXEC                                   */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de file_flags.c   */
int			out_to_file_flags(void);
int			append_to_file_flags(void);
int			heredoc_tmp_file_flags(void);

/*   Fonctions de here_doc_2.c   */
int			is_backslash_at_end(char *str);
int			is_escaped(char *line, int index);
char		*get_variable_value(char *var_name, t_data *data);
void		write_line_to_heredoc(char *line, int heredoc_fd);
void		remove_escape_character(char **line, int index);

/*   Fonctions de here_doc_3.c   */
void		manage_here_doc_process(t_node *node, t_lexer *lexer_lst, \
			t_data *data);

/*   Fonctions de here_doc_condition.c   */
bool		is_heredoc_delimiter_matched(char *delimiter, char *line);
bool		is_heredoc_ended_by_signal(t_data *data);
bool		is_heredoc_interrupted_for_stdin_restore(t_data *data);
bool		is_heredoc_file_opening_failed(int file_descriptor);
bool		is_valid_variable_char(char c);

/*   Fonctions de here_doc.c   */
char		*extract_and_get_var_value(char **line, int var_name_start, \
			int var_name_length, t_data *data);
void		expand_variable(char **line, int index, t_data *data);
void		process_heredoc_line(char **line, t_data *data);
void		ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data);

/*   Fonctions de init_exec_utils_condition.c   */
bool		is_first_input_redirection_error(t_node *node);
bool		is_current_lexer_token_cmd(t_lexer *current_lexer);
bool		is_pipe_or_end_of_lexer_linked_list(t_lexer *element);

/*   Fonctions de init_exec_utils.c   */
int			is_token_type_cmd(t_lexer *lexer_lst);
int			count_pipe_or_end_in_lexer_linked_list(t_lexer *lexer_list);
int			count_cmd_in_lexer_linked_list(t_lexer *lexer_list);
char		*ft_strdup(t_data *data, char *src);
void		handle_redirect_input_error(t_node *node, t_lexer *lexer_lst);

/*   Fonctions de init_exec.c   */
void		ft_init_exec(t_data *data);
void		build_cmd_linked_list(t_node *node, t_data *data, t_exec *utils);
bool		is_there_more_cmds_to_exec(int index, int total_cmds);
bool		is_env_var_list_uninitialized(t_exec *utils);
t_node		*insert_command_at_end_of_linked_list(\
			t_node *node_lst, t_node *new);

/*   Fonctions de init_export_condition.c   */
bool		is_sorting_linked_list_env_var_unnecessary(\
			t_export **head_of_linked_list_env_var);
bool		is_current_value_greater_than_next(\
			char *current_value, char *next_value);

bool		next_element_exists_for_sorting(t_export *current);
bool		is_linked_list_env_var_empty(t_export *head_of_linked_list_env_var);
bool		exist_linked_list_env_var_to_process(t_exec *utils);

/*   Fonctions de init_export.c   */
void		sort_linked_list_env_var(t_export **head_of_linked_list_env_var);
t_export	*ft_new_variable_to_linked_list_env_var(t_data *data, \
			t_export *new);
t_export	*ft_buil_sorted_linked_list_env_var(\
			t_data *data, t_export *head_of_linked_list_env_var);

/*   Fonctions de init_fd_condition_pipe.c   */
bool		is_current_token_pipe(t_lexer *lexer_lst);
bool		is_current_token_not_pipe(t_lexer *lexer_lst);

/*   Fonctions de init_fd_condition.c   */
bool		is_input_fd_open_ready_for_read(t_node *node);
bool		is_append_out_followed_by_fd_token(t_lexer *lex_lst);
bool		is_output_fd_open_for_closing(t_node *node);
bool		is_output_fd_open_for_closing(t_node *node);

/*   Fonctions de init_fd_heredoc_condition.c  */
bool		is_here_doc_followed_by_delimiter(t_lexer *lexer_lst);
bool		is_heredoc_tmp_file_exists(t_node *node);
void		configure_here_doc_input(\
			t_node *node, t_lexer *lex_lst, t_data *data);

/*   Fonctions de init_fd_redirect_condition.c  */
bool		is_redirect_out_followed_by_fd_token(t_lexer *lex_lst);
bool		is_input_redirection_followed_by_token_fd(t_lexer *lexer_lst);
bool		is_output_redirection_error_detected(t_node *node);
bool		is_normal_output_redirection_error_detected(\
			t_node *node, t_lexer *lex_lst);

bool		is_output_append_redirection_error_detected(\
			t_node *node, t_lexer *lex_lst);

/*   Fonctions de init_fd_word_condition.c  */
bool		is_next_word_existing_and_readable(t_lexer *lexer_lst);
bool		is_next_word_missing(t_lexer *lexer_lst);
bool		is_next_lexeme_word_existing(t_lexer *lex_lst);

/*   Fonctions de init_fd.c   */
void		setup_input_redirection(\
			t_node *node, t_lexer *lexer_lst, t_data *data);
void		append_output_redirection(\
			t_node *node, t_lexer *lex_lst, \
			int *is_output_redirection_feasible);
void		normal_output_redirection(t_node *node, t_lexer *lex_lst);
void		setup_output_redirection(t_node *node, t_lexer *lexer_lst);
bool		is_next_command_segment_present(t_lexer *lex_lst);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                  LEXER                                    */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de lex.c   */
int			ft_write_fd(char *str, int fd);
void		add_lexer_to_end(t_data *data, char *str);
void		process_lexer_input(char *str, int *i, int *j, t_quote *state);

/*   Fonctions de lexer_utils.c   */
int			check_redir_error(t_lexer *tmp);
int			count_words_in_input(char *str);
void		assign_command_or_argument_token(t_lexer *tmp, t_lexer *first);
t_lexer		*create_new_lexer(t_data *data, char *str);
t_token		which_redir(t_lexer *tmp);

/*   Fonctions de lexer.c   */
int			get_word_in_list(char *str, int i, t_data *data, t_lexer *tmp);
void		get_data_in_node(t_lexer **lexer_list);
void		get_token_in_node(t_lexer **lexer_list, t_lexer *tmp);
void		ft_init_lexer_process(t_data *data);
void		process_input_string(\
			t_data *data, t_lexer *tmp, t_lexer *current, int i);

/*   Fonctions divers de lexer   */
int			is_white_space(char c);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                 PARSER                                    */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de parser_utils.c   */
int			is_space(char *str, char c);
char		*ft_substr(\
			t_data *data, char const *s, unsigned int start, size_t len);
size_t		ft_strlen3_mini(char const *s);

/*   Fonctions de parser.c   */
int			quotes_check(char *str, t_data *data);
int			redirection_check(char *str);
int			cmd_start_check(char *str);
int			redir_arg_check(t_data *data);
int			ft_parser(t_data *data);

/*   Fonctions de parser2.c   */
int			invalid_pipes_check(t_data *data);
int			nb_brack(char *str);
int			token_error_check(t_data *data);
int			invalid_tokens_check(t_data *data);
int			invalid_tokens_check_2(t_data *data);

/*   Fonctions de parser4.c   */
int			slash_check(char *str, char c);
int			directory_check(char *str);
int			pipe_bracket_check(char *str);
int			err_code_directory(t_data *data);

/*   Fonctions de token_remover.c   */
char		*remove_dbl_quotes(t_data *data, char *tmp);
char		*remove_spl_quotes(t_data *data, char *tmp);
char		*remove_parenthesis(t_data *data, char *tmp);
void		token_remover(t_data *data);
void		token_remover_2(t_data *data);

/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */
/*                                 SIGNALS                                   */
/*   -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-'   */

/*   Fonctions de signal_heredoc.c   */
void		ctrl_c_handler(int sig);
void		ctrl_c_handler_here_doc(int sig);
void		handle_sig(t_data *data);

/*   Fonctions de signal_handler.c   */
void		handle_ctrl_c(int sig);
void		handle_ctrl_backslash(int sig);
void		handle_process_signal(void);

#endif
