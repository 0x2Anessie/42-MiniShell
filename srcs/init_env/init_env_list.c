#include "../../include/minishell.h"

/*
	Calcule la taille d'un tableau de chaînes de caractères représentant les
	variables d'environnement. Cette fonction parcourt le tableau jusqu'à un
	élément NULL, indiquant la fin du tableau, et compte le nombre d'éléments.
 */
int	env_size(char **env)
{
	int	index;

	index = ZERO_INIT;
	while (env[index])
		index++;
	return (index);
}

/*
	Crée un nouvel élément de type t_env pour une variable d'environnement.
	Cette fonction alloue de la mémoire pour un nouvel élément, copie la
	variable d'environnement fournie dans 'content' et initialise le pointeur
	'next' à NULL.
 */
t_env	*create_and_init_env_var_node(t_data *data, char *env)
{
	t_env	*new;

	new = malloc_track(data, sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_env_name_and_value = ft_strdup(data, env);
	new->next_var_env_name_and_value = NULL;
	return (new);
}

/*
	Ajoute un nouvel élément à la fin d'une liste chaînée de type t_env. Cette
	fonction parcourt la liste jusqu'à atteindre le dernier élément, puis y
	attache le nouvel élément, permettant l'extension dynamique de la liste.
 */
void	ft_env_lst_add_to_end(t_env *lst, t_env *new)
{
	t_env	*tmp;

	tmp = lst;
	while (tmp->next_var_env_name_and_value)
		tmp = tmp->next_var_env_name_and_value;
	tmp->next_var_env_name_and_value = new;
}

/*
	Crée une liste chaînée à partir d'un tableau d'environnements.
	Cette fonction convertit les variables d'environnement fournies sous forme
	de tableau de chaînes en une liste chaînée de type t_env, facilitant leur
	manipulation et leur accès au sein du programme.
 */
t_env	*create_env_list_from_array(t_data *data, char **env)
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
		final = init_env_list_with_pwd_if_empty(data, final);
		return (final);
	}
	while (++i < env_nb)
	{
		new = create_and_init_env_var_node(data, env[i]);
		if (!final)
			final = new;
		else
			ft_env_lst_add_to_end(final, new);
	}
	return (final);
}
