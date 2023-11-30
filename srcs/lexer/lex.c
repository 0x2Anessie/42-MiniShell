/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 23:49:02 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/30 01:47:53 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_write_fd(char *str, int fd)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
		i++;
	if (write (fd, str, i) == FAIL)
	{
		ft_printf(ERR_WRIT_NO_SPAC_LEFT_DEVC);
		g_all.utils->err = 1;
		return (1);
	}
	return (0);
}

void	add_lexer_to_end(t_lexer **lexer_list, char *str)
{
	t_lexer	*new;
	t_lexer	*current;

	new = create_new_lexer(str);
	if (*lexer_list == NULL)
	{
		*lexer_list = new;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		current = *lexer_list;
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
		new->next = NULL;
	}
	return ;
}

void	process_lexer_input(char *str, int *i, int *j, t_quote *state)
{
	while (str[*i])
	{
		update_quoting_state(str[*i], state);
		if (!is_white_space(str[*i])
			&& (!state->is_dquote && !state->is_quote))
		{
			(*j)++;
			(*i)++;
		}
		else if (is_white_space(str[*i])
			&& (!state->is_dquote && !state->is_quote))
			break ;
		else if (state->is_dquote || state->is_quote)
		{
			(*j)++;
			(*i)++;
		}
	}
}
