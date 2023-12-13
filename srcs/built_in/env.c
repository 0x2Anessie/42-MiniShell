
#include "../../include/minishell.h"

char	*unset_var(t_lexer *lexer_lst)
{
	t_lexer	*tmp;

	tmp = lexer_lst;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->word, "unset", 5))
		{
			if (tmp->next && tmp->next->word && !(tmp->next->word[0] == '\0'))
			{
				tmp = tmp->next;
				return (tmp->word);
			}
			else
			{
				write(2, "unset: not enough arguments\n", 28);
				break ;
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	env_display(t_lexer *lexer_lst, t_data *data)
{
	t_lexer	*tmp;

	tmp = lexer_lst;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->word, "env"))
		{
			display_env(data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	display_env(t_data *data)
{
	t_env	*current;

	current = data->utils->env_lst;
	if (data->utils->node->out > 0)
	{
		while (current)
		{
			if (ft_write_fd(current->content, data->utils->node->out))
				return ;
			ft_write_fd("\n", data->utils->node->out);
			current = current->next;
		}
	}
	else if (!data->utils->node->out_fail)
	{
		while (current)
		{
			printf("%s\n", current->content);
			current = current->next;
		}
	}
}

void	remove_export_node(t_export *current, t_export *prev, t_data *data)
{
	if (prev == NULL)
		data->utils->export_lst = current->next;
	else
		prev->next = current->next;
}

int	unset_things(t_lexer *lexer_lst, t_data *data)
{
	if (env_display(lexer_lst, data) == 0)
	{
		data->utils->var = NULL;
		data->utils->var = unset_var(lexer_lst);
		if (data->utils->var)
		{
			remove_node(data->utils->var, data);
			remove_node_export(data->utils->var, data);
		}
	}
	globi = 0;
	return (0);
}
