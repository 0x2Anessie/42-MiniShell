#include "../../include/minishell.h"

char	*parse_quote(t_data *data, char *tmp)
{
	int	len;

	len = ft_strlen(tmp);
	if (tmp[0] == '\"' && tmp[1] == '\"')
		return (tmp);
	if (len >= 2 && tmp[0] == '"' && tmp[len - 1] == '"' && is_space(tmp,
			' ') == 0)
	{
		tmp = ft_substr(data, tmp, 1, (size_t)len - 2);
		return (tmp);
	}
	return (tmp);
}

char	*parse_quote2(t_data *data, char *tmp)
{
	int	len;

	len = ft_strlen(tmp);
	if (len >= 2 && tmp[0] == '\'' && tmp[len - 1] == '\'' && is_space(tmp,
			' ') == 0)
	{
		tmp = ft_substr(data, tmp, 1, (size_t)len - 2);
		return (tmp);
	}
	return (tmp);
}

char	*parse_para(t_data *data, char *tmp)
{
	int	len;

	len = ft_strlen(tmp);
	if (len >= 2 && tmp[0] == '(' && tmp[len - 1] == ')')
	{
		tmp = ft_substr(data, tmp, 1, (size_t)len - 2);
		return (tmp);
	}
	return (tmp);
}

void	rm_para_quote(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp)
	{
		tmp->cmd_segment = parse_para(data, tmp->cmd_segment);
		tmp->cmd_segment = parse_quote(data, tmp->cmd_segment);
		tmp = tmp->next;
	}
}

void	rm_para_quote2(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp)
	{
		tmp->cmd_segment = parse_para(data, tmp->cmd_segment);
		tmp->cmd_segment = parse_quote2(data, tmp->cmd_segment);
		tmp = tmp->next;
	}
}
