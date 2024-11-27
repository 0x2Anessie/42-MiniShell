#include "../../include/minishell.h"

/*
	Active ou désactive les indicateurs de guillemets simples ou doubles
	lorsqu'ils sont rencontrés. Si à la fin, les deux indicateurs sont
	identiques (soit tous deux activés ou désactivés), il y a une erreur
	(guillemet non fermé).
*/
int	quotes_check(char *str, t_data *data)
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

/*
	Vérifie si les caractères après un symbole de redirection forment une
	combinaison valide.
*/
int	redirection_check(char *str)
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

/*
	érifie les erreurs avec les pipes (|) et les redirections en début de
	commande. Signale une erreur si un pipe est utilisé seul ou si les
	redirections sont mal formées.
*/
int	cmd_start_check(char *str)
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
		if (!redirection_check(str))
		{
			printf("syntax error near unexpected token\n");
			return (0);
		}
	}
	return (1);
}

/*
	Vérifie que les tokens de redirection sont suivis par des éléments valides.
*/
int	redir_arg_check(t_data *data)
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
			if (tmp == NULL || tmp->cmd_segment[0] == '\0')
			{
				printf("syntax error near unexpected token\n");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	tmp = data->lexer_list;
	return (1);
}

/*
	Fonction principale qui vérifie toutes les potentielles erreurs de
	parsing.
*/
int	ft_parser(t_data *data)
{
	int	count;

	count = 1;
	if (!quotes_check(data->minishell_input_cmd, data))
		count = 0;
	else if (!cmd_start_check(data->minishell_input_cmd))
		count = 0;
	else if (!token_error_check(data))
		count = 0;
	else if (!redir_arg_check(data))
		count = 0;
	else if (!invalid_tokens_check_2(data))
		count = 0;
	else if (!directory_check(data->minishell_input_cmd))
		count = 0;
	else if (!pipe_bracket_check(data->minishell_input_cmd))
		count = 0;
	if (count == 0)
	{
		g_globi = 2;
		return (0);
	}
	return (1);
}
