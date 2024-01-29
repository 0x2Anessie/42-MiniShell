/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_state_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:59 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:24:00 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_quote_not_part_of_string(char current_char, t_quote *state)
{
	return ((current_char == '"' && is_single_quote_closed(state)) \
	|| (current_char == '\'' && is_doubl_quote_closed(state)));
}

bool	is_single_quote_with_double_quote_closed(\
char current_char, t_quote *state)
{
	return (current_char == '\'' && is_doubl_quote_closed(state));
}

bool	is_double_quote_with_single_quote_closed(\
char current_char, t_quote *state)
{
	return (current_char == '"' && is_single_quote_closed(state));
}

bool	is_quote_char_with_closed_state(char current_char, t_quote *state)
{
	return ((current_char == '"' \
	|| current_char == '\'') \
	&& is_doubl_quote_closed(state));
}
