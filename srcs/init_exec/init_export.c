/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:19 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:29:04 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	swap_env_var_values(t_export *current_node, t_export *next_node)
{
	char	*temp;

	temp = current_node->env_var_name_and_value;
	current_node->env_var_name_and_value = next_node->env_var_name_and_value;
	next_node->env_var_name_and_value = temp;
}

void	sort_linked_list_env_var(t_export **head_of_linked_list_env_var)
{
	int			swapped;
	t_export	*current;

	if (is_sorting_linked_list_env_var_unnecessary(\
	head_of_linked_list_env_var))
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *head_of_linked_list_env_var;
		while (!current->next_env_var_name_and_value)
		{
			if (is_current_value_greater_than_next(\
			current->env_var_name_and_value, \
			current->next_env_var_name_and_value->env_var_name_and_value))
			{
				swap_env_var_values(\
				current, current->next_env_var_name_and_value);
				swapped = 1;
			}
			current = current->next_env_var_name_and_value;
		}
	}
}

t_export	*ft_new_variable_to_linked_list_env_var(\
t_data *data, t_export *new)
{
	new = NULL;
	new = (t_export *)malloc_track(data, sizeof(t_export));
	if (!new)
		return (NULL);
	new->env_var_name_and_value = ft_strdup(\
	data, data->utils->linked_list_full_env_var_copy_alpha->\
	var_env_name_and_value);
	new->next_env_var_name_and_value = NULL;
	return (new);
}

t_export	*ft_buil_sorted_linked_list_env_var(\
t_data *data, t_export *head_of_linked_list_env_var)
{
	t_export	*new;
	t_export	*current_new;

	current_new = NULL;
	while (exist_linked_list_env_var_to_process(data->utils))
	{
		new = ft_new_variable_to_linked_list_env_var(data, new);
		if (is_linked_list_env_var_empty(head_of_linked_list_env_var))
		{
			head_of_linked_list_env_var = new;
			current_new = head_of_linked_list_env_var;
		}
		else
		{
			current_new->next_env_var_name_and_value = new;
			current_new = current_new->next_env_var_name_and_value;
		}
		data->utils->linked_list_full_env_var_copy_alpha = \
		data->utils->linked_list_full_env_var_copy_alpha->\
		next_var_env_name_and_value;
	}
	sort_linked_list_env_var(&head_of_linked_list_env_var);
	data->utils->linked_list_full_env_var_copy_alpha = \
	data->utils->head_env_lst;
	return (head_of_linked_list_env_var);
}
