
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
		if (!ft_strncmp(tmp->word, "unset", 5))
		{
			if (tmp->next && tmp->next->word && !(tmp->next->word[0] == '\0'))
			{
				tmp = tmp->next;
				return (tmp->word);
			}
			else
			{
				write(2, "unset: not enough arguments\n", 28);
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

int	check_env(t_lexer *lexer_lst)
{
	t_lexer	*tmp;

	tmp = lexer_lst;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->word, "env"))
		{
			display_env();
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

/*
	Utilise g_all.utils->node->out pour déterminer si l'affichage doit être
	redirigé ou pas
	la fonction parcour la list des variable d'env (env_lst) et les affiche
	suivi d'une
	nouvelle ligne entre chaque
	si il n'y a pas de redirection, elle use printf pour les afficher dans
	le terminal
*/

void	display_env(void)
{
	t_env	*current;

	current = g_all.utils->env_lst;
	if (g_all.utils->node->out > 0)
	{
		while (current)
		{
			if (ft_write_fd(current->content, g_all.utils->node->out))
				return ;
			ft_write_fd("\n", g_all.utils->node->out);
			current = current->next;
		}
	}
	else if (!g_all.utils->node->out_fail)
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
	supprime des listes env_lst et export_lst.
*/

int	get_unset(t_lexer *lexer_lst)
{
	if (check_env(lexer_lst) == 0)
	{
		g_all.utils->var = NULL;
		g_all.utils->var = check_unset(lexer_lst);
		if (g_all.utils->var)
		{
			remove_node(g_all.utils->var);
			remove_node_export(g_all.utils->var);
		}
	}
	g_all.utils->err = 0;
	return (0);
}
