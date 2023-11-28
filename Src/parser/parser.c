
#include "../../Include/minishell.h"

int	ft_check_quotes(char *str, t_data *data)
{
	int	i;

	i = 0;
	data->squote = 0;
	data->dquote = 1;
	while (str[i])
	{
		if (str[i] == '"' && !data->squote && !data->dquote)
			data->dquote = 1;
		else if (str[i] == '"' && !data->squote && data->dquote)
			data->dquote = 0;
		else if (str[i] == '\'' && !data->squote && data->dquote)
			data->squote = 1;
		else if (str[i] == '\'' && data->squote && data->dquote)
			data->squote = 0;
		i++;
	}
	if (data->squote == data->dquote)
	{
		printf("minishell: syntax error quote not closed\n");
		return (0);
	}
	return (1);
}

int	check_redir(char *str)
{
	int	i;

	i = 0;
	if (str[i + 1] == '\0')
		return (0);
	if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '>' && str[i + 1] == '<')
		|| (str[i] == '<' && str[i + 1] == '<') || (str[i] == '<' && str[i + 1] == '>'))
	{
		if (str[i + 2] == '\0')
			return (0);
	}
	return (1);
}

int	base_check(char *str)
{
	int	i;

	i = 0;
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
		if (tmp->token == fleched || tmp->token == Dfleched || tmp->token == flechedg
			|| tmp->token == flecheg || tmp->token == Dflecheg || tmp->token == flechegd)
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
	if (!ft_check_quotes(data->line, data))
		return (0);
	if (!base_check(data->line))
		return (0);
	if (!pipe_parse(data)) // juste pipe
		return (0);
	if (!ft_cloporte(data)) // si nul apres une fonction plus token
		return (0);
	if (!ft_arrow(data))
		return (0);
	if (!ft_tiret(data->line))
		return (0);
	if (!chevron_pipe(data->line)) // plus utile
		return (0);
	return (1);
}
