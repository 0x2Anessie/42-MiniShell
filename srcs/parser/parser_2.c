/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:22:17 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 11:55:49 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Parcourt la liste des tokens et vérifie la syntaxe autour des pipes,
	comme >| ou |<, qui sont incorrects.
*/
int	invalid_pipes_check(t_data *data)
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

/*
	Vérifie qu'il n'y ait pas plus de deux chevrons à la suite.
*/
int	nb_brack(char *str)
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

/*
	Affiche les messages d'erreurs relatifs eux chevrons et aux pipe.
*/
int	token_error_check(t_data *data)
{
	int		k;
	t_lexer	*tmp;

	k = ZERO_INIT;
	tmp = NULL;
	tmp = data->lexer_list;
	while (tmp)
	{
		k = 0;
		if (!nb_brack(tmp->cmd_segment) || !nb_brack(tmp->cmd_segment))
		{
			printf("syntax error near unexpected token\n");
			return (0);
		}
		if (tmp->cmd_segment[k] == '|' && tmp->cmd_segment[k + 1] == '|')
		{
			printf("syntax error near unexpected token\n");
			return (0);
		}
		if (!invalid_pipes_check(data))
			return (0);
		tmp = tmp->next;
	}
	tmp = data->lexer_list;
	return (1);
}

/*
	Vérifie les combinaisons de tokens invalides relatives aux redirections
	et aux heredoc.
*/
int	invalid_tokens_check(t_data *data)
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

/*
	Vérifie les combinaisons de tokens invalides relatives aux pipes.
*/
int	invalid_tokens_check_2(t_data *data)
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
				printf("syntax error near unexpected token\n");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	if (!invalid_tokens_check(data))
	{
		printf("syntax error near unexpected token\n");
		return (0);
	}
	tmp = data->lexer_list;
	return (1);
}
