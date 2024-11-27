#include "../../include/minishell.h"

/*
	Retire les guillemets des segments de commande.
*/
char	*remove_dbl_quotes(t_data *data, char *tmp)
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

/*
	Retire les guillemets simples des segments de commande.
*/
char	*remove_spl_quotes(t_data *data, char *tmp)
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

/*
	Retire les parenthèses des segments de commande.
*/
char	*remove_parenthesis(t_data *data, char *tmp)
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

/*
	Traiter et de retirer les guillemets et les parenthèses des segments
	de commande.
*/
void	token_remover(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp)
	{
		tmp->cmd_segment = remove_parenthesis(data, tmp->cmd_segment);
		tmp->cmd_segment = remove_dbl_quotes(data, tmp->cmd_segment);
		tmp = tmp->next;
	}
}

/*
	Traiter et de retirer les guillemets simples des segments
	de commande.
*/
void	token_remover_2(t_data *data)
{
	t_lexer	*tmp;

	tmp = data->lexer_list;
	while (tmp)
	{
		tmp->cmd_segment = remove_parenthesis(data, tmp->cmd_segment);
		tmp->cmd_segment = remove_spl_quotes(data, tmp->cmd_segment);
		tmp = tmp->next;
	}
}
