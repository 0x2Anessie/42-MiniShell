/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeme_expansion_and_manipulation_utils.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:22 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:47:33 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	reset_quote_states(t_quote *st)
{
	st->singl_quot_status = ZERO_INIT;
	st->doubl_quot_status = ZERO_INIT;
}

void	initialize_expansion_parameters(\
t_expand *exp, t_data *data, t_lexer **expnd, t_quote *st)
{
	exp->length_of_expanded_var_value = ZERO_INIT;
	data->utils->g_signal_in_char_format = \
	convert_int_to_string_with_tracking(data, g_globi);
	exp->value_of_expanded_var_from_env = \
	allocate_memory_for_expanded_word(expnd, st, data->\
	full_env_var_copy_gamma, data);
}
