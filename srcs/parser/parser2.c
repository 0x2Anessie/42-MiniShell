#include "../../include/minishell.h"

int	pipe_parse2(t_data *data)
{
	int		k;
	t_lexer	*tmp;

	k = ZERO_INIT;
	tmp = data->lexer_list;
	while (tmp)
	{
		k = 0;
		if ((tmp->cmd_segment[k] == '>' && tmp->cmd_segment[k + 1] == '|')
			|| (tmp->cmd_segment[k] == '<' && tmp->cmd_segment[k + 1] == '|'))
		{
			printf("minishell: parse error near `|'\n");
			k++;
			return (0);
		}
		if ((tmp->cmd_segment[k] == '|' && tmp->cmd_segment[k + 1] == '<')
			|| (tmp->cmd_segment[k] == '|' && tmp->cmd_segment[k + 1] == '>'))
		{
			printf("minishell: no such file or directory\n");
			k++;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_chevron(char *str)
{
	int	i;
	int	k;

	k = ZERO_INIT;
	i = ZERO_INIT;
	while (str[i])
	{
		if (str[i] == '<' || str[i] == '>')
		{
			k++;
		}
		i++;
	}
	if (k > 2)
		return (0);
	return (1);
}

int	pipe_parse(t_data *data)
{
	int		k;
	t_lexer	*tmp;

	k = ZERO_INIT;
	tmp = NULL;
	tmp = data->lexer_list;
	while (tmp)
	{
		k = 0;
		if (!ft_chevron(tmp->cmd_segment) || !ft_chevron(tmp->cmd_segment))
		{
			printf("syntax error near unexpected token\n");
			return (0);
		}
		if (tmp->cmd_segment[k] == '|' && tmp->cmd_segment[k + 1] == '|')
		{
			printf("syntax error near unexpected token\n");
			return (0);
		}
		if (!pipe_parse2(data))
			return (0);
		tmp = tmp->next;
	}
	tmp = data->lexer_list;
	return (1);
}

int	ft_arrow2(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp != NULL)
	{
		if ((tmp->token == APPEND_OUT || tmp->token == REDIRECT_OUT))
		{
			if (tmp->next->token == HERE_DOC || tmp->next->token == REDIRECT_IN
				|| tmp->next->token == APPEND_OUT
				|| tmp->next->token == REDIRECT_OUT
				|| tmp->next->token == PIPE)
				return (0);
		}
		if (tmp->token == HERE_DOC || tmp->token == REDIRECT_IN)
		{
			if (tmp->next->token == HERE_DOC || tmp->next->token == REDIRECT_IN
				|| tmp->next->token == APPEND_OUT
				|| tmp->next->token == REDIRECT_OUT
				|| tmp->next->token == PIPE)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_arrow(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp != NULL)
	{
		if (tmp->token == PIPE)
		{
			tmp = tmp->next;
			if (tmp == NULL || tmp->token == PIPE \
			|| tmp->cmd_segment[0] == '\0')
			{
				printf("6 syntax error near unexpected token\n");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	if (!ft_arrow2(data))
	{
		printf("syntax error near unexpected token\n");
		return (0);
	}
	tmp = data->lexer_list;
	return (1);
}
