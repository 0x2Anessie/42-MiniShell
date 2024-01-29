/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:09 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:10 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	cette fonction cree un nouveau noeud pour la liste des variable exporter
	alloue de la memoire pour un nouveau noeudt_export
	initialise le noeud et retourne le noeud cree si il y eu un probleme
*/
t_export	*create_node_str_export(char *str, t_data *data)
{
	t_export	*node;

	node = malloc_track(data, sizeof(t_export));
	if (node)
	{
		node->env_var_name_and_value = str;
		node->next_env_var_name_and_value = NULL;
	}
	return (node);
}

/*
	ajoute un nouveau noeud a la fin de la liste, sauf si c'est le premier elle
	cree avec create_node_str et en fait la premiere
*/
void	lst_add_back_export(t_export **head, char *str, t_data *data)
{
	t_export	*current;

	if (*head == NULL)
	{
		*head = create_node_str_export(str, data);
	}
	else
	{
		current = *head;
		while (current->next_env_var_name_and_value != NULL)
		{
			current = current->next_env_var_name_and_value;
		}
		current->next_env_var_name_and_value = \
		create_node_str_export(str, data);
	}
}
