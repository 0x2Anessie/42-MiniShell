#include "../../include/minishell.h"

bool	is_sorting_linked_list_env_var_unnecessary(\
t_export **head_of_linked_list_env_var)
{
	return (!head_of_linked_list_env_var \
	|| !(*head_of_linked_list_env_var) \
	|| !((*head_of_linked_list_env_var)->next_env_var_name_and_value));
}

bool	is_current_value_greater_than_next(\
char *current_value, char *next_value)
{
	return (ft_strcmp(current_value, next_value) > 0);
}

bool	next_element_exists_for_sorting(t_export *current)
{
	return (current != NULL && current->next_env_var_name_and_value != NULL);
}

/*
	Vérifie si la liste des variables d'environnement est vide.
 */
bool	is_linked_list_env_var_empty(t_export *head_of_linked_list_env_var)
{
	return (head_of_linked_list_env_var == NULL);
}

/*
	verifi si la liste des variables existe
 */
bool	exist_linked_list_env_var_to_process(t_exec *utils)
{
	return (utils->linked_list_full_env_var_copy_alpha != NULL);
}
