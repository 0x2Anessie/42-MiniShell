
#include "../../include/minishell.h"

/*
	Utilisé dans la fonction init_env_list elle parcourt le tableau de chaînes
	**env et compte le nombre d'éléments.
*/
int	env_size(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

/*
	Utilisé dans la fonction init_env_list, elle prend une chaîne du tableau
	de chaînes env en paramètre et crée et initialise un nouveau noeud pour 
	chaque chaîne. Elle copie la variable d'environnement fournie dans 
	new->content et met new->next à NULL pour indiquer la fin de la liste. 
*/
t_env	*init_node(char *env)
{
	t_env	*new;

	new = ft_malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->content = ft_strdup(env);
	new->next = NULL;
	return (new);
}

/*
	Utilisée dans la fonction init_env_list, elle ajoute un nouvel élément
	à la fin de le la liste chaînée.
*/
void	init_list(t_env *lst, t_env *new)
{
	t_env	*tmp;

	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
	Utilisé par la fonction init_env.
	Met toutes les variables d'environnement sous forme de liste
	chaînée plutôt que de tableau. Dans le cas ou aucune variable
	n'est fournie, on utilise if_no_env, sinon tant qu'on a
	pas traité toutes les variables on initialise les noeuds et on
	les ajoute à la chaîne.
	On retourne final, qui est la liste complète.
*/
t_env	*init_env_list(char **env)
{
	int		env_nb;
	int		i;
	t_env	*new;
	t_env	*final;

	i = -1;
	new = NULL;
	final = NULL;
	env_nb = env_size(env);
	if (env_nb == 0)
	{
		final = if_no_env(final);
		return (final);
	}
	while (++i < env_nb)
	{
		new = init_node(env[i]);
		if (!final)
			final = new;
		else
			init_list(final, new);
	}
	return (final);
}
