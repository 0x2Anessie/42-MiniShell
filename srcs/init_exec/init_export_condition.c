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

/**
 * Checks if there exists a next element in the list for sorting purposes.
 *
 * @param current A pointer to the current environment variable node in the list.
 * @return true if a next element exists, false otherwise.
 */
bool	next_element_exists_for_sorting(t_export *current)
{
	return (current != NULL && current->next_env_var_name_and_value != NULL);
}

/**
 * Vérifie si la liste liée des variables d'environnement est vide.
 *
 * @param head_of_linked_list_env_var Pointeur vers la tête de la liste liée
 * des variables d'environnement.
 * @return true si la liste est vide, false si elle contient au moins un élément.
 */
bool	is_linked_list_env_var_empty(t_export *head_of_linked_list_env_var)
{
	return (head_of_linked_list_env_var == NULL);
}

/**
 * Vérifie si la liste liée des variables d'environnement à traiter existe.
 *
 * @param utils Structure contenant la liste liée des variables d'environnement.
 * @return true si la liste liée existe et doit être traitée, false sinon.
 */
bool	exist_linked_list_env_var_to_process(t_exec *utils)
{
	return (utils->linked_list_full_env_var_copy_alpha != NULL);
}
