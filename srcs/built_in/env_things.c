
#include "../../include/minishell.h"

void	remove_node(char *var, t_data *data)
{
	t_env	*current;
	t_env	*prev;

	current = data->utils->env_lst;
	prev = NULL;
	while (current != NULL)
	{
		if (!ft_strncmp(current->content, var, ft_strlen4(var)))
		{
			remove_env_node(current, prev, data);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	remove_env_node(t_env *current, t_env *prev, t_data *data)
{
	if (!prev)
		data->utils->env_lst = current->next;
	else
		prev->next = current->next;
}

void	remove_node_export(char *var, t_data *data)
{
	t_export	*current;
	t_export	*prev;

	current = data->utils->export_lst;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->value, var, ft_strlen4(var)) == 0)
		{
			remove_export_node(current, prev, data);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
