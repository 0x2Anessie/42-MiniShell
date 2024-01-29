/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dual_quote_status_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:22:36 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:22:38 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_both_quotes_off(t_quote *state)
{
	return (state->doubl_quot_status == DOUBL_QUOT_CLOS \
	&& state->singl_quot_status == SIMPL_QUOT_CLOS);
}

int	is_only_single_quote_on(t_quote *state)
{
	return (state->doubl_quot_status == DOUBL_QUOT_CLOS \
	&& state->singl_quot_status == SIMPL_QUOT_OPEN);
}

int	is_only_double_quote_on(t_quote *state)
{
	return (state->doubl_quot_status == DOUBL_QUOT_OPEN \
	&& state->singl_quot_status == SIMPL_QUOT_CLOS);
}

int	is_both_quotes_on(t_quote *state)
{
	return (state->doubl_quot_status == DOUBL_QUOT_OPEN \
	&& state->singl_quot_status == SIMPL_QUOT_OPEN);
}
