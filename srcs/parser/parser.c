#include "../../include/minishell.h"

int	ft_check_quotes(char *str, t_data *data)
{
	int	i;

	i = ZERO_INIT;
	data->is_sing_quot = 0;
	data->is_doub_quot = 1;
	while (str[i])
	{
		if (str[i] == '"' && !data->is_sing_quot && !data->is_doub_quot)
			data->is_doub_quot = 1;
		else if (str[i] == '"' && !data->is_sing_quot && data->is_doub_quot)
			data->is_doub_quot = 0;
		else if (str[i] == '\'' && !data->is_sing_quot && data->is_doub_quot)
			data->is_sing_quot = 1;
		else if (str[i] == '\'' && data->is_sing_quot && data->is_doub_quot)
			data->is_sing_quot = 0;
		i++;
	}
	if (data->is_sing_quot == data->is_doub_quot)
	{
		printf("minishell: syntax error quote not closed\n");
		return (0);
	}
	return (1);
}

int	check_redir(char *str)
{
	int	i;

	i = ZERO_INIT;
	if (str[i + 1] == '\0')
		return (0);
	if ((str[i] == '>' && str[i + 1] == '>') \
	|| (str[i] == '>' && str[i + 1] == '<') \
	|| (str[i] == '<' && str[i + 1] == '<') \
	|| (str[i] == '<' && str[i + 1] == '>'))
	{
		if (str[i + 2] == '\0')
			return (0);
	}
	return (1);
}

int	base_check(char *str)
{
	int	i;

	i = ZERO_INIT;
	if (str[0] == '|' && str[1] == '|')
	{
		printf("minishell: syntax error near unexpected token `||'\n");
		return (0);
	}
	if (str[0] == '|')
	{
		printf("minishell:  syntax error near unexpected token `|'\n");
		return (0);
	}
	if (str[i] == '<' || str[i] == '>')
	{
		if (!check_redir(str))
		{
			printf("1 syntax error near unexpected token\n");
			return (0);
		}
	}
	return (1);
}

int	ft_cloporte(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp != NULL)
	{
		if (tmp->token == REDIRECT_OUT || tmp->token == APPEND_OUT \
		|| tmp->token == REDIRECT_OUTIN || tmp->token == REDIRECT_IN \
		|| tmp->token == HERE_DOC || tmp->token == REDIRECT_INOUT)
		{
			tmp = tmp->next;
			if (tmp == NULL || tmp->word[0] == '\0')
			{
				printf("2 syntax error near unexpected token\n");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	tmp = data->lexer_list;
	return (1);
}

int	ft_parser(t_data *data)
{
	if (!ft_check_quotes(data->minishell_input_cmd, data))
		return (0);
	if (!base_check(data->minishell_input_cmd))
		return (0);
	if (!pipe_parse(data))
		return (0);
	if (!ft_cloporte(data))
		return (0);
	if (!ft_arrow(data))
		return (0);
	if (!ft_tiret(data->minishell_input_cmd))
		return (0);
	if (!chevron_pipe(data->minishell_input_cmd))
		return (0);
	return (1);
}
