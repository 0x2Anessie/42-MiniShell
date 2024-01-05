#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <stdio.h>
# include <readline/history.h>
# include "../Lib/includes/lib.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdbool.h>

/* STATUS AND FLAG DEFINITIONS */
# define FAIL -1
# define TRUE 1
# define FALSE 0
# define ZERO_INIT 0
# define EXIT_SUCCESS 0
# define FT_FAILURE_EXIT 0
# define CHILD_PROCESS 0
# define FORK_FAILURE -1
# define FORWARD_OFFSET 1
# define BACKWARD_OFFSET -1
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
# define QUOTED 1
# define NOT_QUOTED 0
# define EXPANSION_REQUIRED 1
# define NO_ACTION_REQUIRED 0
# define INVALID_PIPE -2

# define OUTPUT_REDIRECTION_ACCESS_FAIL_VALUE 1
# define OUTPUT_REDIRECTION_TARGET_MISSING_VALUE 2

/* OUTPUT REDIRECTION ERROR ID */
# define OUTPUT_TARGET_ACCESS_ERROR_CODE 1
# define OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE 2

/* INPUT_FD_VALUE */
# define INPUT_FD_NOT_SET -1
# define INPUT_FD_REDIRECTION_FAIL -2

/* OUTPUT_FD_VALUE */
# define OUTPUT_FD_NOT_CONFIGURED -1


# define CMD_NOT_FOUND 0
# define CMD_FOUND 1
# define SIGNAL_EXIT_OFFSET 128

/* FILE AND STREAM CONSTANTS */
# define TAB '\t'  // Tabulation horizontale
# define SHIFT_OUT 14  // SO (Shift Out)
# define HEREDOC_TEMP_FILE "here_doc.tmp"
# define PERM_RWX_ALL 0777
# define PERM_O_RW_G_R_OT_R 0644
# define OUT_FILE "Outfile"

/* ERROR MESSAGES AND SIGNALS */
# define ERR_HEREDOC_EOF_WARNING "bash: warning: here-document delimited by \
end-of-file (wanted `EOF')\n"
# define ERR_WRIT_NO_SPAC_LEFT_DEVC "write error: no space left on device\n"
# define ERR_MSG_CMD_NOT_FOUND ": command not found\n"
# define ERR_AMB_REDIRECT "bash: ambiguous redirect\n"
# define ERR_MEMORY_ALLOCATION "Erreur lors de l'allocation de mémoire"
# define WRITE_ERROR_MSG "write error"
# define QUIT_MESSAGE "Quit\n"
# define CTRL_C_SIGINT SIGINT
# define CTRL_BACKSLSH SIGQUIT
# define IGNORE_SIG_ACTION SIG_IGN

/* PIPE AND PROCESS CONSTANTS */
# define PIPE_READ_END 0
# define PIPE_WRITE_END 1

/* SHELL COMMAND AND TOKEN CONSTANTS */
# define TOKEN_IS_COMMAND 1
# define TOKEN_IS_PIPE 0
# define INFINITY_LOOP 1

/* STRING AND CHARACTER CONSTANTS */
# define FT_NEWLINE "\n"
# define IS_SEPARATOR 1
# define NOT_SEPARATOR 0
# define SLASH_FOUND 0
# define NO_SLASH_FOUND 1
# define SIMPL_QUOT_CLOS 0
# define SIMPL_QUOT_OPEN 1
# define DOUBL_QUOT_CLOS 0
# define DOUBL_QUOT_OPEN 1

/*   Single quote started status   */
# define SINGLE_QUOTE_NOT_STARTED 0
# define SINGLE_QUOTE_STARTED 1

/*  DEFINE CMD BUILING  */
# define CMD_ECHO "echo"
# define CMD_ENV_VARS "env"
# define CMD_EXIT_SHELL "exit"
# define CMD_UNSET_VARS "unset"
# define CMD_EXPORT_VARS "export"
# define CMD_CHANG_DIRCT "cd"
# define CMD_PRINT_DIRCT "pwd"
# define CMD_CHANGE_DIRECTORY "chdir"
# define CMD_UNSET_ENV_VAR "unset"

/* ENVIRONMENT VARIABLE SET */
# define ENV_SET_CURRENT_WORKING_DIR "PWD="
# define ENV_SET_USER_HOME_DIR "HOME="
# define ENV_SET_OLDPWD "OLDPWD="
# define ENV_SET_PATH_PREFIX "PATH="

/* ENVIRONMENT VARIABLE NAMES */
# define ENV_PREVIOUS_WORKING_DIR "OLDPDW"
# define ENV_CURRENT_WORKING_DIR "PWD"
# define ENV_USER_HOME_DIR "HOME"

# define WELCOME_MSG "\
\n\033[93m\
╔════════════════════════════════════════════════════════════════════════════╗\n\
║                                                                            ║\n\33[93m\
║                 \033[31m▄▀▄ █▄ █ ▄▀▄ █ █▀    █   █ █ █▄▀ ▄▀▄ █▀                    \033[93m║\n\
║                 \033[31m█▀█ █ ▀█ █▀█ █ ▄█ ▄  █▄▄ █▄█ █▀▄ █▀█ ▄█                    \033[93m║\n\
║                                   \033[31m▄▀                                       \033[93m║\n\
║                                                                            \033[93m║\n\
║                 \033[31m█▀█ ▄▀▄ █ █ █      █▀▄ ▄▀▄ ▄▀▀ ▄▀▄ █▀▀ █                   \033[93m║\n\
║                 \033[31m█▀▀ █▀█ █▄█ █▄▄ ▄  █▀▄ █▀█ █▀▀ █▀█ ██▄ █▄▄                 \033[93m║\n\
║                                \033[31m▄▀                                          \033[93m║\n\
║                                                                            \033[93m║\n\
║                         \033[31m█▀█ █▀▄ █▀▀ █▀ █▀▀ █▄░█ ▀█▀                        \033[93m║\n\
║                         \033[31m█▀▀ █▀▄ ██▄ ▄█ ██▄ █░▀█ ░█                         \033[93m║\n\
║                                                                            \033[93m║\n\
║                                                                            \033[93m║\n\
║     \033[34m███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░     \033[93m║\n\
║     \033[34m████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░     \033[93m║\n\
║     \033[34m██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░     \033[93m║\n\
║     \033[34m██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░     \033[93m║\n\
║     \033[34m██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗     \033[93m║\n\
║     \033[34m╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝     \033[93m║\n\
║                                                                            \033[93m║\n\
╚════════════════════════════════════════════════════════════════════════════╝\
\n\033[m"

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
}			t_token;

typedef struct s_list_node
{
	char				*data;
	struct s_list_node	*next;
}						t_list_node;

typedef struct s_node
{
	int				input_fd;
	int				output_fd;
	char			*heredoc_tmp_fullname;
	int				here_doc_fd;
	int				index;
	int				is_command_present;
	int				is_output_redirection_feasible;
	int				is_input_redirection_failed;
	int				output_redirection_error_id;
	struct s_node	*next;
}			t_node;

typedef struct s_env
{
	char			*content;/*     Change name    */
	struct s_env	*next;
}				t_env;

typedef struct s_stringdata
{
	int		len;/*     Change name    */
	int		i;/*     Change name    */
	int		j;/*     Change name    */
	int		is_escaped;/*     Change name    */
}			t_stringdata;

typedef struct s_lexer
{
	char			*word;/*     Change name    */
	t_token			token;
	int				i;/*     Change name    */
	int				nb_words;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}			t_lexer;

typedef struct s_quote
{
	int	singl_quot_status;
	int	doubl_quot_status;
	int	singl_quot_start_status;
	int	found;/*     Change name    */
}				t_quote;

typedef struct s_expand
{
	char	*str;/*     Change name    */
	int		len;/*     Change name    */
	int		found;/*     Change name    */
	int		need_expand;/*     Change name    */
	int		quote;/*     Change name    */
	int		i;/*     Change name    */
}				t_expand;

typedef struct s_export
{
	char			*value;
	struct s_export	*next;
}			t_export;

typedef struct s_exec
{
	int				can_run;/*     Change name    */
	char			*hd;/*     Change name    */
	int				dupin;/*     Change name    */
	int				is_here_doc;
	char			**env;
	int				err;/*     Change name    */
	t_env			*env_lst;/*     Change name    */
	int				nb_cmd;/*     Change name    */
	int				nb_node;/*     Change name    */
	char			*error;/*     Change name    */
	int				ret;/*     Change name    */
	int				previous_fd;
	char			*var;/*     Change name    */
	t_node			*head_node_lst;/*     Change name    */
	t_lexer			*head_lexer_lst;/*     Change name    */
	t_export		*export_lst;/*     Change name    */
	t_export		*head_export_lst;/*     Change name    */
	t_env			*head_env_lst;/*     Change name    */
	struct s_node	*node;
	struct s_exec	*next;
	struct s_exec	*prev;
}				t_exec;

typedef struct s_memory/*     Change name s_trash_memory    */
{
	void			*add;
	struct s_memory	*next;
}			t_memory;/*     Change name t_trash_memory   */

typedef struct s_data
{
	char		*test;/*     Change name    */
	int			x;/*     Change name    */
	int			squote;/*     Change name    */
	int			dquote;/*     Change name    */
	int			ac;/*     Change name    */
	int			nb_mots;
	char		**av;
	char		**env;
	char		**nv;/*     Change name    */
	char		*line;/*     Change name    */
	int			index;/*     Change name    */
	char		**allcommand;/*     Change name    */
	char		**envpaths;/*     Change name    */
	char		*finalpath;/*     Change name    */
	t_exec		*utils;/*     Change name    */
	t_memory	*trash_memory;
	t_lexer		*lexer_list;
	t_quote		*quote;
	t_list_node	*envp;
	t_expand	*expand;

}			t_data;

extern unsigned int	g_signal_received;

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                    MAIN                                   */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de main.c */
void		prompt_loop(char *tmp, t_data *data, char **env);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                  BUILT_IN                                 */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de built_in_utils_2.c */
int			ft_string_length(char *s);
char		*ft_strjoin_with_memory_tracking(char *s1, char *s2, t_data *data);
size_t		ft_str_len_until_equal(char *str);
char		*ft_strjoin_free_arg2_with_memory_tracking(\
char *s1, char *s2, t_data *data);

/* Déclarations des fonctions de built_in_utils.c */
char		*ft_strcpy(char *dest, const char *src);
char		*add_quotes_string(char *str, t_data *data);
char		*join_them(char *str, int i, int flag, t_data *data);
char		*case_egale(char *str, t_data *data);
void		exit_all(t_data *data);

/* Déclarations des fonctions de cd.c */
int			verif_pwd(char *str, t_data *data);
char		*get_home(t_env	*tmp, t_data *data);
int			verif_home(char *str, t_data *data);
int			change_directory_for_home(t_env *tmp, t_data *data);

/* Déclarations des fonctions de cd2.c */
int			change_directory_for_pwd(t_env *tmp, t_data *data);
char		*create_new_var(char *str, t_data *data);
void		get_cd(t_lexer *lexer_lst, t_data *data);
int			wrong_cd(t_lexer *lexer_lst);
int			cd_with_arg(t_data *data, char *path, char *old, int *i);

/* Déclarations des fonctions de cd3.c */
int			change_directory(char *dir);
int			change_directory_for_oldpwd(t_env *tmp, t_data *data);
char		*get_pwd_env(t_env	*tmp, t_data *data);
int			verif_oldpwd(char *str, t_data *data);

/* Déclarations des fonctions de echo_utils.c */
void		freetab(char **tab, int k);
int			ft_word_count(char *s, char c);
char		*ft_put_word_in_malloc(char *s, char c, char *tab);
char		**ft_malloc_word(char **tab, char *s, char c, t_data *data);
char		**ft_split_mini(char const *s, char c, t_data *data);

/* Déclarations des fonctions de echo.c */
int			ft_strcmp(char *s1, char *s2);
void		display_echo_arg(char **tab, int i, t_data *data);
void		get_words(t_lexer *lexer_lst, char **tab, int *i);
void		process_echo(char **tab, int i, t_data *data);
void		procc_in_echo_std(char **tab, int i, t_data *data);

/* Déclarations des fonctions de echo2.c */
void		init_echo(t_lexer *lexer_lst, t_data *data);
int			check_n(char *str);
void		simulate_echo(char **tab, t_data *data);

/* Déclarations des fonctions de env_create.c */
t_env		*create_node_str(char *str, t_data *data);
void		lst_add_back(t_exec *utils, char *str, t_data *data);

/* Déclarations des fonctions de env_things.c */
void		remove_node(char *var, t_data *data);
void		remove_env_node(t_env *current, t_env *prev, t_data *data);
void		remove_node_export(char *var, t_data *data);
void		remove_export_node(\
t_export *current, t_export *prev, t_data *data);

/* Déclarations des fonctions de env.c */
char		*check_unset(t_lexer *lexer_lst);
int			check_env(t_lexer *lexer_lst, t_data *data);
void		display_env(t_data *data);
int			get_unset(t_lexer *lexer_lst, t_data *data);

/* Déclarations des fonctions de export_checking.c */
void		export_quotes(t_export *don, t_data *data);
int			check_case_with_char(char *str);
int			check_case(char *str);
void		supp_quote_and_add_env(t_exec **utils, t_lexer *tmp, t_data *data);

/* Déclarations des fonctions de export_create.c */
t_export	*create_node_str_export(char *str, t_data *data);
void		lst_add_back_export(t_export **head, char *str, t_data *data);

/* Déclarations des fonctions de export_parsing_2.c */
int			is_number(char c);
int			check_parsing_char(char *str, int i);
int			check_parsing_error(char *str);
int			check_parsing_export(char *str);

/* Déclarations des fonctions de export_parsing.c */
void		initialize_data(t_stringdata *data, char *str);
void		extract_within_quotes(t_stringdata *data, char *str);
void		extract_quotes(t_stringdata *data, char *str, char quote);
void		remove_single_quotes(char *str);
void		remove_double_quotes(char *str);

/* Déclarations des fonctions de export_var_exist.c */
int			verif_var_exist_export(t_exec *utils, char *str, t_data *data);
int			verif_equal(char *str, char c);
int			verif_var_exist(t_exec *utils, char *str);
int			verif_var_exist_export_not_maj(t_exec *utils, char *str);

/* Déclarations des fonctions de export.c */
void		print_export(t_export *export_lst, t_data *data);
int			export_parsing_syntaxe(t_lexer *tmp, t_data *data);
void		process_word_and_add_export(t_lexer *tmp, t_data *data);
void		export_remaining(t_lexer *tmp, t_data *data);
int			export_things(t_lexer *lexer_lst, t_data *data);

/* Déclarations des fonctions de ft_exit.c */
int			is_valid_number2(int neg, char *str, int i);
int			is_valid_number(char *str);
void		free_res(int *fd, t_data *data);
void		ft_exit_with_code(\
t_lexer *lex, int *fd, unsigned char exit_code, t_data *data);

void		ft_exit(t_lexer *lex, int *fd, t_data *data);

/* Déclarations des fonctions de path_cd.c */
int			verif_oldpwd_export(char *str, t_data *data);
int			verif_pwd_export(char *str, t_data *data);

/* Déclarations des fonctions de pwd.c */
int			get_pwd(char *tab, t_data *data);
void		display_pwd(t_data *data);
void		display_pwd_error(t_data *data);
void		find_old_pwd(t_env	*env, t_data *data);

/* Déclarations des fonctions de write_echo.c */
void		write_echo_fd(char **tab, int i, t_data *data);
int			procc_in_echo_fd(char **tab, int i, int j, t_data *data);

/* Déclarations des fonctions divers de built_in */
char		*ft_strjoin2_mini(char *s1, char *s2, t_data *data);
char		*get_old_pwd(t_env	*tmp, t_data *data);
void		execute_lexer_command_with_args(t_data *data);
void		ft_putstr_fd_mini(char *s1, char *s2, int fd, t_data *data);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                    EXEC                                   */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

void		initialize_pid_array_to_zero(pid_t *pid, int nb_node);
void		init_fd_and_status_vars(int *fd, int *y, int *wstatus);
pid_t		create_and_execute_child_process(\
t_data *data, int *fd, int count, t_exec utils);

char		**build_arg_array_from_lexer(t_data *data);
int			check_if_cmd_full_path_exec_is_valid(\
t_lexer *lexer, t_exec utils, t_data *data);

char		*find_command_full_path(char *cmd, t_env *env_lst, t_data *data);
char		**extract_paths_from_env(t_env *env_lst, t_data *data);
void		configure_stdout_redirection_for_command(\
t_exec utils, int *fd, int count);

void		configure_stdin_redirection_for_command(t_exec utils, int count);
int			count_args_until_pipe_for_cmd_array(t_lexer *lexer_list);
t_lexer		*reaches_next_cmd_preceded_by_pipe(t_lexer *lexer_list);
int			close_file_descriptors_in_linked_list(t_data *data);
int			closes_ends_of_pipe(int *fd);
int			check_for_slash_path_delimiter(t_lexer *lexer);
//int			ft_exit_child(t_exec *utils, int *fd);
void		exec_cmd_with_redirection_and_builtins(\
t_data *data, int *fd, int count, t_exec utils);

/* Déclarations des fonctions de command_analysis.c */
int			is_exact_command_match(\
t_lexer *lexer_lst, const char *command, int command_length);

int			is_built_in_command(t_lexer *lexer_lst);
int			should_continue_execution(t_data *data, int *y);
int			check_redirection_validity_in_node(t_node *node);
t_lexer		*find_next_command_in_lexer(t_lexer *lexer_list);

/* Déclarations des fonctions de command_execution.c */
void		ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data);
void		close_fds_if_needed(int *fd, int previous_fd);
int			manage_exec_linked_cmd_sequence(\
int *fd, pid_t *pid, t_data *data, int *y);

void		wait_and_process_child_statuses(\
pid_t *pid, int *wstatus, int nb_node, t_data *data);

void		manage_execution_resources(t_data *data);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                  EXPANDER                                 */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/*   Déclarations des fonctions de dual_quote_status_utils.c   */
int			is_both_quotes_off(t_quote *state);
int			is_only_single_quote_on(t_quote *state);
int			is_only_double_quote_on(t_quote *state);
int			is_both_quotes_on(t_quote *state);

/*   Déclarations des fonctions de env_var_expansion_length.c   */
int			get_env_var_expansion_length(\
char *word, int *length, char *env_var, t_quote *state);

int			calculate_length_until_single_quote(char *word, int *length);
int			calculate_length_for_env_expansion(\
char *word, char **env_var, t_quote *state, int *length);

int			calculate_expanded_env_var_length(\
char *word, char **env_var, t_quote *state, int *length);

/*   Déclarations des fonctions de expand_utils.c   */
void		reset_quoting_state(t_quote *state);
int			length_of_number_for_string_conversion(long n);
char		*convert_negative_int_to_string_with_tracking(\
t_data *data, char *res, long n, int length);

char		*convert_int_to_string_with_tracking(t_data *data, int n);
int			is_decimal_digit(char c);

/*   Déclarations des fonctions de expansion_processing.c  */
void		expand_variables_and_handle_special_chars(\
t_lexer **expnd, t_quote *state, t_expand *exp, t_data *data);

void		length_of_env_variable_value(\
t_lexer **exp, t_quote *state, t_data *data, int *expanded_length);

int			get_env_var_expansion_length(\
char *word, int *length, char *env_var, t_quote *state);

/*   Déclarations des fonctions de malloc_expand.c  */
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

/*   Déclarations des fonctions de quot_state_validations.c */
int			is_single_quote_started(t_quote *state);
int			is_single_quote_started_double_quote_closed(t_quote *state);
int			is_single_quote_open_and_started(t_quote *state);

/*   Déclarations des fonctions de lexeme_expansion_and_manipulation.c */
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

/*   Déclarations des fonctions de shell_variable_handling.c */
int			append_chars_expnt_until_singl_quot(char *word, t_expand *exp);
int			handle_unfound_expansion_word(char *w, t_quote *state);
int			append_curnt_error_code_to_expansion_struc(\
t_expand *exp, t_data *data);

int			find_and_expand_env_var_with_special_char(\
char *w, t_expand *exp, t_data *data, t_quote *state);

int			expand_env_vars_with_question_mark_handling(\
char *w, t_expand *exp, t_data *data, t_quote *state);

/*   Déclarations des fonctions de shell_word_expansion.c */
int			determine_expansion_or_quote_removal(\
t_lexer **to_check, t_quote *state, t_expand *exp, t_data *data);

bool		should_expand(\
t_lexer *tmp, t_quote *state, t_expand *exp, t_data *data);

void		expand(t_quote *state, char **env, t_lexer *tmp, t_data *data);

/*   Déclarations des fonctions de singular_quote_status_utils.c   */
int			is_single_quote_open(t_quote *state);
int			is_single_quote_closed(t_quote *state);
int			is_doubl_quote_open(t_quote *state);
int			is_doubl_quote_closed(t_quote *state);

/*   Déclarations des fonctions de special_char_handling.c   */
int			is_next_char_end_or_special(char *word, t_quote *state);
int			is_next_char_decimal_digit(char *word);
int			is_special_char_found_with_state_not_found(\
t_lexer *exp, t_quote *state, int index);

int			is_next_char_question_mark(t_lexer *exp, int index);

/*   Déclarations des fonctions de special_character_checks.c  */
int			is_special_syntax_character(char c, t_quote *st);
int			is_dollar_or_doubl_or_singl_quote(char *word);
int			is_dollar_at_end(char *str);

/*   Déclarations des fonctions de split_word_by_quotes.c   */
int			count_subwords_with_quote_states(char *word, t_quote *state);
char		**split_word_by_quotes(t_data *data, char *word, t_quote *state);

/*   Déclarations des fonctions de state_quote.c  */
void		update_quoting_state(char c, t_quote *state);
int			is_char_cmd_separator(char c);
int			is_end_of_command_word(char *cmd, int i, t_quote *state);
int			count_string_array_elements(char **arr);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                     FREE                                  */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/*   Déclarations des fonctions de free.c  */
int			ft_exit_child(t_exec *utils, int *fd, t_data *data);
void		add_to_trash_memory(t_data *data, void *add);
void		*ft_malloc_with_tracking(t_data *data, size_t size);
void		ft_free(void *add, t_data *date);
void		ft_free_all(t_data *data);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                  INIT_ENV                                 */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/*   Déclarations des fonctions de init_env_list.c  */
int			env_size(char **env);
t_env		*create_and_init_env_var_node(t_data *data, char *env);
void		ft_env_lst_add_to_end(t_env *lst, t_env *new);
t_env		*create_env_list_from_array(t_data *data, char **env);

/*   Déclarations des fonctions de init_env.c */
void		malloc_no_env_initial_node(t_data *data, char *str, t_env **env);
t_env		*init_env_list_with_pwd_if_empty(t_data *data, t_env *final);
t_exec		*init_env(t_data *data, char **env);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                 INIT_EXEC                                 */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de file_flags.c */
int			out_to_file_flags(void);
int			append_to_file_flags(void);
int			heredoc_tmp_file_flags(void);

/* Déclarations des fonctions de here_doc.c */
void		write_line_to_heredoc(char *line, int heredoc_fd);
void		ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data);
void		manage_here_doc_process(\
t_node *node, t_lexer *lexer_lst, t_data *data);

/* Déclarations des fonctions de init_exec_utils.c */
void		handle_redirect_input_error(t_node *node, t_lexer *lexer_lst);
char		*ft_strdup(t_data *data, char *src);
int			is_token_type_cmd(t_lexer *lexer_lst);
int			nb_cmd(t_lexer *lexer_list);
int			nb_node(t_lexer *lexer_list);

/* Déclarations des fonctions de init_exec_utils.c */
t_node		*insert_command_at_end_of_linked_list(\
t_node *node_lst, t_node *new);

void		build_cmd_linked_list(t_node *node, t_data *data, t_exec *utils);
void		ft_init_exec(t_data *data);

/* Déclarations des fonctions de init_export.c */
void		sort_export_lst(t_export **head_export);
t_export	*ft_new_export_node(t_data *data, t_export *new);
t_export	*ft_buil_sorted_chained_list_env_var(\
t_data *data, t_export *export_lst);

/* Déclarations des fonctions de init_fd.c */
void		configure_here_doc_input(\
t_node *node, t_lexer *lex_lst, t_data *data);

void		setup_input_redirection(\
t_node *node, t_lexer *lexer_lst, t_data *data);

void		append_output_redirection(\
t_node *node, t_lexer *lex_lst, int *is_output_redirection_feasible);

void		normal_output_redirection(t_node *node, t_lexer *lex_lst);
void		setup_output_redirection(t_node *node, t_lexer *lexer_lst);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                    LEXER                                  */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de lex.c */
int			ft_write_fd(char *str, int fd);
void		add_lexer_to_end(t_data *data, char *str);
void		process_lexer_input(char *str, int *i, int *j, t_quote *state);

/* Déclarations des fonctions de lexer_utils.c */
t_lexer		*create_new_lexer(t_data *data, char *str);
int			check_redir_error(t_lexer *tmp);
t_token		which_redir(t_lexer *tmp);
void		assign_command_or_argument_token(t_lexer *tmp, t_lexer *first);
int			count_words_in_input(char *str);

/* Déclarations des fonctions de lexer.c */
void		get_data_in_node(t_lexer **lexer_list);
void		get_token_in_node(t_lexer **lexer_list, t_lexer *tmp);
int			get_word_in_list(char *str, int i, t_data *data, t_lexer *tmp);
void		process_input_string(\
t_data *data, t_lexer *tmp, t_lexer *current, int i);

void		ft_init_lexer_process(t_data *data);

/* Déclarations des fonctions divers de lexer */
int			is_white_space(char c);
// void		print_lex(t_data *data);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                   PARSER                                  */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de parser_utils.c */
size_t		ft_strlen3_mini(char const *s);
char		*ft_substr(\
t_data *data, char const *s, unsigned int start, size_t len);

int			is_space(char *str, char c);

/* Déclarations des fonctions de parser.c */
int			ft_check_quotes(char *str, t_data *data);
int			check_redir(char *str);
int			base_check(char *str);
int			ft_cloporte(t_data *data);
int			ft_parser(t_data *data);

/* Déclarations des fonctions de parser2.c */
int			pipe_parse2(t_data *data);
int			ft_chevron(char *str);
int			pipe_parse(t_data *data);
int			ft_arrow2(t_data *data);
int			ft_arrow(t_data *data);

/* Déclarations des fonctions de parser4.c */
int			stop_pand_slash(char *str, char c);
int			ft_tiret(char *str);
int			chevron_pipe(char *str);

/* Déclarations des fonctions de rm_para_quote.c */
char		*parse_quote(t_data *data, char *tmp);
char		*parse_quote2(t_data *data, char *tmp);
char		*parse_para(t_data *data, char *tmp);
void		rm_para_quote(t_data *data);
void		rm_para_quote2(t_data *data);

/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */
/*                                   SIGNALS                                 */
/* -'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-',-' */

/* Déclarations des fonctions de ctrl_c_signals.c */
void		ctrl_c_handler(int sig);
void		ctrl_c_handler_here_doc(int sig, t_data *data);
void		handle_sig(t_data *data);

/* Déclarations des fonctions de error_signals.cc */
void		handle_process_signal(void);

/* Déclarations des fonctions de signal_exits.c */
void		handle_ctrl_c(int sig);
void		handle_ctrl_backslash(int sig);
int			exit_stat_ctrl_backslash_sigquit(void);
int			exit_stat_ctrl_c_sigint(void);

#endif
