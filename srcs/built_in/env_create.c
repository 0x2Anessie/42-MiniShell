/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 18:25:15 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/30 01:47:53 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_node_str(char *str)
{
	t_env	*node;

	node = ft_malloc_with_tracking(sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = str;
	node->next = NULL;
	return (node);
}

void	lst_add_back(t_exec *utils, char *str)
{
	t_env	*current;
	t_env	*head;

	head = utils->head_env_lst;
	if (head == NULL)
	{
		head = create_node_str(str);
	}
	else
	{
		current = head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = create_node_str(str);
	}
}
