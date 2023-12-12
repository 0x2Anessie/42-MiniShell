#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <stdio.h>
# include <readline/history.h>
# include "../Lib/includes/lib.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

/*  MACRO ACTUELLEMENT UTILISER  */
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
# define OUT_FAIL 2
# define TAB '\t'  // Tabulation horizontale
# define SHIFT_OUT 14  // SO (Shift Out)
# define TOKEN_IS_COMMAND 1
# define TOKEN_IS_PIPE 0
# define INFINITY_LOOP 1
# define PERM_O_RW_G_R_OT_R 0644
# define ERR_HEREDOC_EOF_WARNING "bash: warning: here-document delimited by \
end-of-file (wanted `EOF')\n"
# define HEREDOC_TEMP_FILE "here_doc.tmp"
# define PERM_RWX_ALL 0777
# define ERR_WRIT_NO_SPAC_LEFT_DEVC "write error: no space left on device\n"
# define ERR_MSG_CMD_NOT_FOUND ": command not found\n"
# define ERR_AMB_REDIRECT "bash: ambiguous redirect\n"
# define OUT_FILE "Outfile"
# define OUT_TO_FILE_FLAGS (O_CREAT | O_RDWR | O_TRUNC)
# define CMD_NOT_FOUND 0
# define CMD_FOUND 1
# define SIGNAL_EXIT_OFFSET 128
# define EXIT_STAT_CTRL_BACKSLSH_SIGQUIT (SIGNAL_EXIT_OFFSET + SIGQUIT)
# define EXIT_STAT_CTRL_C_SIGINT (SIGNAL_EXIT_OFFSET + SIGINT)
# define CTRL_C SIGINT
# define CTRL_BACKSLSH SIGQUIT
# define IGNORE_SIG_ACTION SIG_IGN
# define PIPE_READ_END 0
# define PIPE_WRITE_END 1
# define APPND_TO_FILE_FLAGS (O_CREAT | O_RDWR | O_APPEND)
# define HEREDOC_TMPFILE_FLAGS (O_CREAT | O_WRONLY | O_TRUNC)
# define ERR_MEMORY_ALLOCATION "Erreur lors de l'allocation de mémoire"
# define WRITE_ERROR_MSG "write error"
# define OLDPWD_PREFIX "OLDPWD="
# define QUIT_MESSAGE "Quit\n"
# define PWD_PREFIX "PWD="
# define HOME_PREFIX "HOME="
# define PATH_PREFIX "PATH="
# define FT_NEWLINE "\n"

/*  DEFINE CMD BUILING  */
# define CMD_ECHO "echo"
# define CMD_ENV_VARS "env"
# define CMD_EXIT_SHELL "exit"
# define CMD_UNSET_VARS "unset"
# define CMD_EXPORT_VARS "export"
# define CMD_CHANG_DIRCT "cd"
# define CMD_PRINT_DIRCT "pwd"

# define WELCOME_MSG "\n\033[93m╔════════════════════════════════════════════════════════════════════════════╗\n║                                                                            ║\n\33[93m║\33[93m                  \033[31m▄▀▄ █▄ █ ▄▀▄ █ █▀    █   █ █ █▄▀ ▄▀▄ █▀                   \033[93m║\n║                  \033[31m█▀█ █ ▀█ █▀█ █ ▄█ ▄  █▄▄ █▄█ █▀▄ █▀█ ▄█                   \033[93m║\n║                                   \033[31m▄▀                                       \033[93m║\n║                                                                            \033[93m║\n║                 \033[31m█▀█ ▄▀▄ █ █ █      █▀▄ ▄▀▄ ▄▀▀ ▄▀▄ █▀▀ █                   \033[93m║\n║                 \033[31m█▀▀ █▀█ █▄█ █▄▄ ▄  █▀▄ █▀█ █▀▀ █▀█ ██▄ █▄▄                 \033[93m║\n║                                \033[31m▄▀                                          \033[93m║\n║                                                                            \033[93m║\n║                       \033[31m█▀█ █▀▄ █▀▀ █▀ █▀▀ █▄░█ ▀█▀ █▀                       \033[93m║\n║                       \033[31m█▀▀ █▀▄ ██▄ ▄█ ██▄ █░▀█ ░█░ ▄█                       \033[93m║\n║                                                                            \033[93m║\n║                                                                            \033[93m║\n║     \033[34m███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░     \033[93m║\n║     \033[34m████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░     \033[93m║\n║     \033[34m██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░     \033[93m║\n║     \033[34m██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░     \033[93m║\n║     \033[34m██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗     \033[93m║\n║     \033[34m╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝     \033[93m║\n║                                                                            \033[93m║\n╚════════════════════════════════════════════════════════════════════════════╝\n\033[m"

extern struct s_all	g_all;

typedef enum s_token
{
	CMD,
	ARG,
	fleched,
	Dfleched,
	flecheg,
	HERE_DOC,
	flechegd,
	flechedg,
	FD,
	DELIMITER,
	PIPE,
}			t_token;

typedef struct s_ListNode
{
	char				*data;
	struct s_ListNode	*next;
}						t_ListNode;

typedef struct s_node
{
	int				in;
	int				out;
	char			*here_doc;
	int				here_doc_fd;
	int				index;
	int				has_cmd;
	int				has_out;
	int				in_fail;
	int				out_fail;
	struct s_node	*next;
}			t_node;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}				t_env;

typedef struct s_stringdata
{
	int		len;
	int		i;
	int		j;
	int		is_escaped;
}			t_stringdata;

typedef struct s_lexer
{
	char			*word;
	t_token			token;
	int				i;
	int				nb_words;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}			t_lexer;

typedef struct s_quote
{
	int	is_quote;
	int	is_dquote;
	int	sq_first;
	int	found;
}				t_quote;

typedef struct s_expand
{
	char	*str;
	int		len;
	int		found;
	int		need_expand;
	int		quote;
	int		i;
}				t_expand;

typedef struct s_export
{
	char			*value;
	struct s_export	*next;
}			t_export;

typedef struct s_exec
{
	int				can_run;
	char			*hd;
	int				dupin;
	int				is_here_doc;
	char			**env;
	int				err;
	t_env			*env_lst;
	int				nb_cmd;
	int				nb_node;
	char			*error;
	int				ret;
	int				previous_fd;
	char			*var;
	t_node			*head_node_lst;
	t_lexer			*head_lexer_lst;
	t_export		*export_lst;
	t_export		*head_export_lst;
	t_env			*head_env_lst;
	struct s_node	*node;
	struct s_exec	*next;
	struct s_exec	*prev;
}				t_exec;

typedef struct s_memory
{
	void			*add;
	struct s_memory	*next;
}			t_memory;

typedef struct s_all
{
	t_exec		*utils;
	t_memory	*d_mem;
}				t_all;

typedef struct s_data
{
	char		*test;
	int			squote;
	int			dquote;
	int			ac;
	int			nb_mots;
	char		**av;
	char		**env;
	char		**nv;
	char		*line;
	int			index;
	char		**allcommand;
	char		**envpaths;
	char		*finalpath;
	t_memory	*memory;
	t_lexer		*lexer_list;
	t_quote		*quote;
	t_ListNode	*envp;
	t_expand	*expand;

}			t_data;

////////////////////////LEXER_LIST////////////////////////////////////
void		ft_init_lexer_process(t_data *data);
int			ft_write_fd(char *str, int fd);
int			get_word_in_list(char *str, int i, t_data *data,
				t_lexer *tmp);
void		get_token_in_node(t_lexer **lexer_list, t_lexer *tmp);
void		get_data_in_node(t_lexer **lexer_list);
void		add_lexer_to_end(t_data *data, char *str);
int			count_words_in_input(char *str);
int			is_white_space(char c);
t_token		which_redir(t_lexer *tmp);
int			check_redir_error(t_lexer *tmp);
t_lexer		*create_new_lexer(t_data *data, char *str);
void		assign_command_or_argument_token(t_lexer *tmp, t_lexer *first);
void		print_lex(t_data *data);

///////////////////////PARSING/////////////////////////////////////////
int			ft_parser(t_data *data);
int			ft_cloporte(t_data *data);
int			base_check(char *str);
int			check_redir(char *str);
int			ft_check_quotes(char *str, t_data *data);
int			ft_arrow(t_data *data);
int			ft_arrow2(t_data *data);
int			pipe_parse(t_data *data);
int			ft_chevron(char *str);
int			pipe_parse2(t_data *data);
size_t		ft_strlen3_mini(char const *s);
char		*ft_substr(t_data *data, char const *s, 
				unsigned int start, size_t len);
int			is_space(char *str, char c);
void		rm_para_quote(t_data *data);
char		*parse_para(t_data *data, char *tmp);
char		*parse_quote(t_data *data, char *tmp);
char		*parse_quote2(t_data *data, char *tmp);

///////////////////////INIT_ENV////////////////////////////////////////
t_exec		*init_env(t_data *data, char **env);
t_env		*create_env_list_from_array(t_data *data, char **env);
void		malloc_no_env_initial_node(t_data *data, char *str, t_env **env);
void		ft_env_lst_add_to_end(t_env *lst, t_env *new);
t_env		*create_and_init_env_var_node(t_data *data, char *env);
int			env_size(char **env);
t_env		*init_env_list_with_pwd_if_empty(t_data *data, t_env *final);

///////////////////////SIGNALS///////////////////////////////////////////
void		handle_sig(void);
void		ctrl_c_handler_here_doc(int sig);
void		ctrl_c_handler(int sig);
void		handle_process_signal(void);

///////////////////////INIT_EXEC/////////////////////////////////////////
void		ft_init_exec(t_data *data);
void		get_all_node(t_node *node, t_data *data, t_exec *utils);
void		setup_output_redirection(t_node *node, t_lexer *lexer_lst);
void		setup_input_redirection(t_node *node, t_lexer *lexer_lst);
void		configure_here_doc_input(t_node *node, t_lexer *lex_lst);
void		get_in_fail(t_node *node, t_lexer *lexer_lst);
void		Dflechedput_redirection(t_node *node, t_lexer *lex_lst, int *has_out);
t_node		*ft_add_back_node(t_node *node_lst, t_node *new);
int			nb_node(t_lexer *lexer_list);
int			nb_cmd(t_lexer *lexer_list);
int			has_cmd(t_lexer *lexer_lst);
void		manage_here_doc_process(t_node *node, t_lexer *lexer_lst);
void		ft_read_input(t_node *node, t_lexer *lexer_lst);
char		*ft_strdup(t_data *data, char *src);
t_export	*get_export_list(t_data *data, t_export *export_lst);
t_export	*ft_new_export_node(t_data *data, t_export *new);
void		sort_export_lst(t_export **head_export);

///////////////////////////BUILT IN///////////////////////////////////////
int			get_pwd(char *tab, t_data *data);
char		*ft_strjoin2_mini(char *s1, char *s2, t_data *data);
int			procc_in_echo(char **tab, int i, int j);
void		simulate_echo(t_lexer *lexer_lst, t_data *data);
void		simu_echo(char **tab, int i);
int			ft_strcmp(char *s1, char *s2);
char		**ft_split_mini(char const *s, char c, t_data *data);
char		**ft_malloc_word(char **tab, char *s, char c, t_data *data);
char		*ft_put_word_in_malloc(char *s, char c, char *tab);
int			ft_word_count(char *s, char c);
void		freetab(char **tab, int k);
void		get_cd(t_lexer *lexer_lst, t_data *data);
int			verif_pwd_export(char *str, t_data *data);
int			verif_oldpwd_export(char *str, t_data *data);
int			verif_oldpwd(char *str, t_data *data);
int			verif_pwd(char *str, t_data *data);
char		*var_exist(char *str, t_data *data);
int			change_directory(char *dir);
size_t		ft_strlen_eguale(char *str);
char		*ft_strjoin_2(char *s1, char *s2, t_data *data);
char		*case_egale(char *str, t_data *data);
char		*ft_strjoin4(char *s1, char *s2, t_data *data);
int			ft_strlen4(char *s);
char		*ft_strcpy(char *dest, const char *src);
char		*add_quotes_string(char *str, t_data *data);
int			unset_things(t_lexer *lexer_lst);
char		*unset_var(t_lexer *lexer_lst);
void		remove_node_export(char *var);
void		remove_node(char *var);
int			export_things(t_lexer *lexer_lst, t_data *data);
void		export_quotes(t_export *don, t_data *data);
int			check_case(char *str);
void		print_export(t_export *export_lst);
void		export_remaining(t_lexer *tmp, t_data *data);
int			check_parsing_export(char *str);
void		process_word(t_exec **utils, t_lexer *tmp, t_data *data);
void		remove_double_quotes(char *str);
void		remove_single_quotes(char *str);
int			verif_var_exist_export(t_exec *utils, char *str, t_data *data);
int			verif_var_exist(t_exec *utils, char *str);
int			verif_equal(char *str, char c);
void		lst_add_back(t_exec *utils, char *str, t_data *data);
void		lst_add_back_export(t_export **head, char *str, t_data *data);
t_export	*create_node_str_export(char *str, t_data *data);
int			is_number(char c);
int			check_parsing_2(char *str);
void		ft_exit(t_lexer *lex, int *fd, t_data *data);
void		display_pwd(void);
void		display_pwd_error(void);
void		display_env(void);
int			check_case_2(char *str);
void		remove_export_node(t_export *current, t_export *prev);
void		remove_env_node(t_env *current, t_env *prev);
int			wrong_cd(t_lexer *lexer_lst);
void		process_echo_2(char **tab, int i);
void		simulate_echo_3(char **tab);
int			cd_2(t_data *data, char *path, char *old, int *i);
void		write_echo(char **tab, int i);
void		exec_chemin(t_data *data);
void		ft_putstr_fd_mini(char *s1, char *s2, int fd, t_data *data);

/* Déclarations des fonctions de command_analysis.c */
int			is_command_equal(t_lexer *lexer_lst, const char *command, int command_length);
int			is_built_in(t_lexer *lexer_lst);
int			should_continue_execution(t_all *g_all, int *y);
int			is_valid_redirection(t_node *node);
t_lexer		*find_next_command(t_lexer *lexer_list);

/* Déclarations des fonctions de command_execution.c */
void		ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data);
void		close_fds_if_needed(int *fd, int previous_fd);
int			start_exec(int *fd, pid_t *pid, t_data *data, int *y);
void		wait_child_processes(pid_t *pid, int *wstatus, int nb_node);
void		ft_prep_exec(t_data *data);

///////////////////////////EXEC///////////////////////////////////////////
void		ft_bzero_pid_array(pid_t *pid, int nb_node);
void		init_var(int *fd, int *y, int *wstatus);
pid_t		ft_child(t_data *data, int *fd, int count, t_exec utils);
char		**get_arg(t_data *data);
int			check_path_exec(t_lexer *lexer, t_exec utils, t_data *data);
char		*get_cmd_path(char *cmd, t_env *env_lst, t_data *data);
char		**get_path(t_env *env_lst, t_data *data);
void		handle_out(t_exec utils, int *fd, int count);
void		handle_in(t_exec utils, int count);
int			ft_nb_arg(t_lexer *lexer_list);
t_lexer		*go_next_cmd(t_lexer *lexer_list);
int			verif_var_exist_export_2(t_exec *utils, char *str);
int			close_fd(void);
int			close_pipe(int *fd);
int			slashinlex(t_lexer *lexer);
//int			ft_exit_child(t_exec *utils, int *fd);
void		child_of_chill(t_data *data, int *fd, int count, t_exec utils);
////////////////expand
int			ft_isdigit(char c);
int			single_quote_expantion(char *word, t_expand *exp);
int			word_not_found(char *w, t_quote *state);
int			put_error_code(t_expand *exp);
int			search_word(char *w, t_expand *exp, char *nv, t_quote *state);
int			basic_expantion(char *w, t_expand *exp, char **nv, t_quote *state);
int			is_special_syntax_character(char c, t_quote *st);
int			need_expand_or_rm_quote(t_lexer **to_check, t_quote *state,
				t_expand *exp, t_data *data);
void		expand(t_quote *state, char **env, t_lexer *tmp, t_data *data);
void		reset_quoting_state(t_quote *state);
int			search_in_env_len(char *w, char **nv, t_quote *st, int *len);
void		sum_expansion_length(t_lexer **expnd, t_quote *st,
				t_expand *exp, t_data *data);
void		expansion_length_for_word(t_lexer **exp, t_quote *st, char **env,
				int *len);
char		*malloc_for_expand(t_lexer **exp, t_quote *st, char **env, t_data *data);
void		manage_expantion(t_lexer **expnd, t_quote *st, t_data *data,
				t_expand *exp);
void		add_back_new_node(char **insert, t_lexer *back, t_data *data,
				int len);
void		replace_old_node(t_lexer **old_node, char **to_insert, t_data *data);
char		*remove_quote(char *old, t_quote *state, t_data *data);
int			on_word(char *word, int index, t_quote *state);
char		*copy_word(char *word, t_quote *state, char *str);
char		*malloc_word(char *word, t_quote *state, t_data *data);
int			len_d_array(char *word, t_quote *state);
char		**split_word(t_data *data, char *word, t_quote *state);
int			next_squote(char *cmd);
int			next_dquote(char *cmd);
int			next_squote_len(char *cmd, int i, t_quote *state);
int			next_dquote_len(char *cmd, int i, t_quote *state);
void		update_quoting_state(char c, t_quote *state);
int			sep_word(char c);
int			end_word(char *cmd, int i, t_quote *state);
int			len_darr(char **arr);
int			put_error_code(t_expand *exp);
char		*malloc_word_without_quote(int len, char *old, t_quote *state, t_data *data);
char		*put_word_without_quote(char *old, char *nw, t_quote *state);
char		*ft_itoa_mini(t_data *data, int n);

/////////////////FREE
int			ft_exit_child(t_exec *utils, int *fd, t_data *data);
void		check_memory_allocation(t_data *data, void *add);
void		*ft_malloc_with_tracking(t_data *data, size_t size);
void		ft_free(void *add, t_data *date);
void		ft_free_all(t_data *data);
int			ft_len_nb(long n);
char		*ft_is_neg_mini(t_data *data, char *res, long n, int i);
void		prompt_loop(char *tmp, t_data *data, char **env);
int			dollar_at_end(char *str);
int			ft_tiret(char *str);
int			chevron_pipe(char *str);
void		rm_para_quote2(t_data *data);
void		rm_pwd(void);
void		find_old_pwd(t_env	*env);
int			change_directory2(t_env *tmp);
int			verif_home(char *str, t_data *data);
int			change_directory3(t_env *tmp);
char		*get_old_pwd(t_env	*tmp);
char		*get_pwd_env(t_env	*tmp);
int			change_directory4(t_env *tmp);
int			count_quote(char *str);
void		get_words(t_lexer *lexer_lst, char **tab, int *i);
void		process_echo(char **tab, int i);
void		exit_all(t_data *data);
void		process_lexer_input(char *str, int *i, int *j, t_quote *state);

#endif
