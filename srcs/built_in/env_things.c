
#include "../../include/minishell.h"

/*
	cette fonction parcourt la env_lst pour trouver un noeud donc le contenu contient
	var puis utilise remove_env_node pour supprimer le noeud trouver
*/
void	remove_node(char *var, t_data *data)
{
	t_env	*current;
	t_env	*prev;

	current = data->utils->linked_list_full_env_var_copy_alpha;
	prev = NULL;
	while (current != NULL)
	{
		if (!ft_strncmp(current->var_env_name_and_value, var, ft_string_length(var)))/*         ---> condition non intelligible --> fonction         */
		{
			remove_env_node(current, prev, data);
			return ;
		}
		prev = current;
		current = current->next_var_env_name_and_value;
	}
}

/*
	Si le nœud à supprimer est le premier de la liste (prev est NULL), met à jour la tête
	de la liste. Sinon, ajuste le pointeur next du nœud précédent pour exclure le nœud courant.
*/
void	remove_env_node(t_env *current, t_env *prev, t_data *data)
{
	if (!prev)
		data->utils->linked_list_full_env_var_copy_alpha = current->next_var_env_name_and_value;
	else
		prev->next_var_env_name_and_value = current->next_var_env_name_and_value;
}

/*
	fonctionne comme remove_node mais cette fois il parcour la head_of_linked_list_env_var a la recherche
	d'un noeud correspondant a var pui appel remove_export_node pour le supprimer
*/
void	remove_node_export(char *var, t_data *data)
{
	t_export	*current;
	t_export	*prev;

	current = data->utils->head_of_linked_list_env_var;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->env_var_name_and_value, var, ft_string_length(var)) == 0)/*         ---> condition non intelligible --> fonction         */
		{
			remove_export_node(current, prev, data);
			return ;
		}
		prev = current;
		current = current->next_env_var_name_and_value;
	}
}

/*
	Si le nœud à supprimer est le premier de la liste (prev est NULL), met à jour la tête
	de la liste. Sinon, ajuste le pointeur next du nœud précédent pour exclure le nœud courant.
*/
void	remove_export_node(t_export *current, t_export *prev, t_data *data)
{
	if (prev == NULL)
		data->utils->head_of_linked_list_env_var = current->next_env_var_name_and_value;
	else
		prev->next_env_var_name_and_value = current->next_env_var_name_and_value;
}
