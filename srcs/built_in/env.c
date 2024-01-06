
#include "../../include/minishell.h"

/*
	cette fonction parcour la lexer_lst jusqu'a trouver unset puis si unset est suivie
	d'un argument valide elle retourne cette arguement
	si aucun arguement valide est trouver elle renvoie une erreur et s'arrete
*/
char	*check_unset(t_lexer *lexer_lst)
{
	t_lexer	*tmp;

	tmp = lexer_lst;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->word, CMD_UNSET_ENV_VAR, 5))/*         ---> condition non intelligible --> fonction         */
		{
			if (tmp->next && tmp->next->word && !(tmp->next->word[0] == '\0'))/*         ---> condition non intelligible --> fonction         */
			{
				tmp = tmp->next;
				return (tmp->word);
			}
			else
			{
				write(STDERR_FILENO, "unset: not enough arguments\n", 28);
				break ;
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

/*
	cette fonction parcour la list du lexer jusqu'a trouver env
	puis si il est trouver appel display_env pour affiche les variable d'env
*/
int	check_env(t_lexer *lexer_lst, t_data *data)
{
	t_lexer	*tmp;

	tmp = lexer_lst;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->word, CMD_ENV_VARS))/*         ---> condition non intelligible --> fonction         */
		{
			display_env(data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

/*
	Utilise g_all.utils->node->output_fd pour déterminer si l'affichage doit être redirigé ou pas
	la fonction parcour la list des variable d'env (env_lst) et les affiche suivi d'une
	nouvelle ligne entre chaque
	si il n'y a pas de redirection, elle use printf pour les afficher dans le terminal
*/
void	display_env(t_data *data)
{
	t_env	*current;

	current = data->utils->env_lst;
	if (data->utils->node->output_fd > 0)/*         ---> condition non intelligible --> fonction         */
	{
		while (current)
		{
			if (ft_write_fd(current->content, data->utils->node->output_fd))/*         ---> condition non intelligible --> fonction         */
				return ;
			ft_write_fd("\n", data->utils->node->output_fd);
			current = current->next;
		}
	}
	else if (!data->utils->node->output_redirection_error_id)/*         ---> condition non intelligible --> fonction         */
	{
		while (current)
		{
			printf("%s\n", current->content);
			current = current->next;
		}
	}
}

/*
	gere la command unset en supprimant les variables d'env et les variable exporter specifier
	la fonction verifie si la commande est env et si oui elle l'affiche
	Si la command n'est pas env, elle trouve la variable à supprimer avec check_unset et la
	supprime des listes env_lst et head_of_linked_list_env_var.
*/
int	get_unset(t_lexer *lexer_lst, t_data *data)
{
	if (check_env(lexer_lst, data) == 0)
	{
		data->utils->var = NULL;
		data->utils->var = check_unset(lexer_lst);
		if (data->utils->var)
		{
			remove_node(data->utils->var, data);
			remove_node_export(data->utils->var, data);
		}
	}
	g_signal_received = 0;
	return (0);
}
