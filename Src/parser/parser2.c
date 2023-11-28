
#include "../../include/minishell.h"

int	pipe_parse2(t_data *data)
{
	int		k;
	t_lexer	*tmp;

	k = 0;
	tmp = data->lexer_list;
	while (tmp)
	{
		k = 0;
		if ((tmp->word[k] == '>' && tmp->word[k + 1] == '|') // egale juste > 
			|| (tmp->word[k] == '<' && tmp->word[k + 1] == '|'))
		{
			printf("KKKKKK zsh: parse error near `|'\n");
			k++;
			return (0);
		}
		if ((tmp->word[k] == '|' && tmp->word[k + 1] == '<')
			|| (tmp->word[k] == '|' && tmp->word[k + 1] == '>')) // supp ou cree le doss
		{
			printf("zsh: no such file or directory\n");
			k++;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_chevron(char *str, char c)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
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

	k = 0;
	tmp = NULL;
	tmp = data->lexer_list;
	while (tmp)
	{
		k = 0;
		if (!ft_chevron(tmp->word, '<') || (!ft_chevron(tmp->word, '>')))
		{
			printf("3 syntax error near unexpected token\n");
			return (0);
		}
		if (tmp->word[k] == '|' && tmp->word[k + 1] == '|')
		{
			printf("3 syntax error near unexpected token\n");
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
		if ((tmp->token == Dfleched || tmp->token == fleched))
		{
			if (tmp->next->token == Dflecheg || tmp->next->token == flecheg
				|| tmp->next->token == Dfleched || tmp->next->token == fleched
				|| tmp->next->token == PIPE)
				return (0);
		}
		if (tmp->token == Dflecheg || tmp->token == flecheg)
		{
			if (tmp->next->token == Dflecheg || tmp->next->token == flecheg
				|| tmp->next->token == Dfleched || tmp->next->token == fleched
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
			if (tmp == NULL || tmp->token == PIPE || tmp->word[0] == '\0')
			{
				printf("6 syntax error near unexpected token\n");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	if (!ft_arrow2(data))
	{
		printf("4 syntax error near unexpected token\n");
		return (0);
	}
	tmp = data->lexer_list;
	return (1);
}
