#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/minishell.h"

/*
	La variable globale "globi" a comme role de renvoyer les codes de retours
	des commandes. Ces codes de retour indiquent si une commande a été exécutée
	avec succès ou s'il y a eu une erreur et, dans ce dernier cas, quelle était
	la nature de l'erreur.
*/
unsigned int	g_globi = ZERO_INIT;

/*
	Responsable de l'initialisation de la structure 't_data', qui contient
	toutes les données principales utilisées par le programme.
*/
void	init_data(t_data *data, int ac, char **av, char **env)
{
	data->command_line_arg_count = ac;
	data->command_line_args = av;
	data->full_env_var_copy_alpha = env;
	data->trash_memory = NULL;
	data->utils = NULL;
	data->minishell_input_cmd = NULL;
	data->lexer_list = NULL;
}

/*
	Utilisée pour renvoyer un message d'erreur sur la sortie d'erreur dans le
	cas ou l'utilisateur essaierait de lancer le programme avec d'autres
	arguments que "./minishell"
*/
void	exit_error(char *str)
{
	ft_write_fd(str, STDERR_FILENO);
	exit(1);
}

/*
	Parcourt la liste chaînée 'env_lst' contenant les variables
	d'environnement et en construit un tableau de chaînes de caractères.
	Ce tableau est utilisé pour passer l'environnement
	aux processus enfants créés par le shell.
*/
char	**get_new_env(t_data *data, t_env *env_lst)
{
	char	**new_env;
	int		i;
	t_env	*head;

	head = env_lst;
	i = ZERO_INIT;
	while (env_lst)
	{
		i++;
		env_lst = env_lst->next_var_env_name_and_value;
	}
	new_env = malloc_track(data, sizeof(char *) * \
				(i + 1));
	if (!new_env)
		return (NULL);
	new_env[i] = NULL;
	env_lst = head;
	i = 0;
	while (env_lst)
	{
		new_env[i++] = env_lst->var_env_name_and_value;
		env_lst = env_lst->next_var_env_name_and_value;
	}
	env_lst = head;
	return (new_env);
}

/*
	Traite la ligne de commande saisie par l'utilisateur (tmp), la parse
	et lui alloue de la memoire.
	Initialise l'environnement et les variables du lexer.
	Fait l'expand puis execute les commandes.
*/
void	prompt_loop(char *tmp, t_data *data, char **env)
{
	t_lexer	*tmp_lex;
	char	**new_env;
	char	*processed_cmd;

	(void)env;
	if (!tmp)
		exit_all(data);
	if (tmp && tmp[0])
	{
		processed_cmd = add_spaces_around_operators(data, tmp);
		data->minishell_input_cmd = processed_cmd;
		ft_init_lexer_process(data);
		if (!err_code_directory(data) || !ft_parser(data))
			return ;
		tmp_lex = data->lexer_list;
		new_env = get_new_env(\
		data, data->utils->linked_list_full_env_var_copy_alpha);
		expand(data->quote, new_env, tmp_lex, data);
		if (tmp_lex && tmp_lex->cmd_segment)
		{
			ft_init_exec(data);
			manage_execution_resources(data);
		}
	}
}

/*
	La fonction 'main' est le point d'entrée du shell minishell. Elle
	initialise les structures de données, configure l'environnement,
	et entre dans une boucle infinie pour lire et traiter les commandes
	saisies par l'utilisateur.
	--> Utilise readline pour recuperer la commande dans un fichier
		temporaire.
*/
int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*tmp;

	if (ac != 1)
		exit_error("bad number of arguments");
	init_data(&data, ac, av, env);
	data.utils = init_env(&data, env);
	tmp = NULL;
	data.utils->head_of_linked_list_env_var = NULL;
	while (42)
	{
		free(tmp);
		tmp = NULL;
		handle_sig(&data);
		tmp = readline("minishell$ ");
		add_history(tmp);
		prompt_loop(tmp, &data, env);
	}
}
