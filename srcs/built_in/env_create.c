
#include "../../include/minishell.h"

t_env	*create_node_str(char *str, t_data data)
{
	t_env	*node;

	node = ft_malloc_with_tracking(data, sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = str;
	node->next = NULL;
	return (node);
}

void	lst_add_back(t_exec *utils, char *str, t_data data)
{
	t_env	*current;
	t_env	*head;

	head = utils->head_env_lst;
	if (head == NULL)
	{
		head = create_node_str(str, data);
	}
	else
	{
		current = head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = create_node_str(str, data);
	}
}
