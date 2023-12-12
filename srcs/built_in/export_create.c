
#include "../../include/minishell.h"

t_export	*create_node_str_export(char *str, t_data *data)
{
	t_export	*node;

	node = ft_malloc_with_tracking(data, sizeof(t_export));
	if (node)
	{
		node->value = str;
		node->next = NULL;
	}
	return (node);
}

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
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = create_node_str_export(str, data);
	}
}
