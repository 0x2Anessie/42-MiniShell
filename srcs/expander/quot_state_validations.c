/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quot_state_validations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:44 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:23:48 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_single_quote_started(t_quote *state)
{
	return (state->singl_quot_start_status == SINGLE_QUOTE_STARTED);
}

int	is_single_quote_started_double_quote_closed(t_quote *state)
{
	return (is_single_quote_started(state) && is_doubl_quote_closed(state));
}

int	is_single_quote_open_and_started(t_quote *state)
{
	return (is_single_quote_open(state) && is_single_quote_started(state));
}
