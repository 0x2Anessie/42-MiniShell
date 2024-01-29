/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_processing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:02 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:46:10 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_char_to_expanded_variable(t_expand *exp, t_lexer *expnd, int index)
{
	exp->value_of_expanded_var_from_env[exp->\
	length_of_expanded_var_value++] = expnd->cmd_segment[index];
}

void	expand_variables_and_handle_special_chars(\
t_lexer **expnd, t_quote *state, t_expand *exp, t_data *data)
{
	int	index;

	index = ZERO_INIT;
	while ((*expnd)->cmd_segment[index])
	{
		update_quoting_state((*expnd)->cmd_segment[index], state);
		if (is_current_char_dollar_sign(*expnd, index))
		{
			if (is_next_char_end_space_dash_or_special(*expnd, state, index))
				add_char_to_expanded_variable(exp, *expnd, index);
			else if (is_next_char_decimal_digit(&(*expnd)->cmd_segment[index]))
				index++;
			else if (is_single_quote_open_and_started(state))
				index += append_chars_expnt_until_singl_quot(\
				&(*expnd)->cmd_segment[index], exp) IDX_ADJUST;
			else
				index += expand_env_vars_with_question_mark_handling(\
				&(*expnd)->cmd_segment[index], exp, data, state) IDX_ADJUST;
		}
		else
			add_char_to_expanded_variable(exp, *expnd, index);
		index++;
	}
}

void	update_lengths_for_dollar_question_mark(\
int *expanded_length, int *index, t_data *data)
{
	*expanded_length += ft_strlen(data->utils->g_signal_in_char_format);
	*index += SKIP_DOLLAR_QUESTION_MARK;
}

void	length_of_env_variable_value(\
t_lexer **exp, t_quote *state, t_data *data, int *expanded_length)
{
	int	index;

	index = ZERO_INIT;
	while ((*exp)->cmd_segment[index])
	{
		update_quoting_state((*exp)->cmd_segment[index], state);
		if (is_current_char_dollar_sign(*exp, index))
		{
			if (is_next_char_question_mark(*exp, index))
				update_lengths_for_dollar_question_mark(\
				expanded_length, &index, data);
			else
				index += calculate_expanded_env_var_length(\
				&(*exp)->cmd_segment[index], \
				data->full_env_var_copy_alpha, state, expanded_length);
			if (is_special_char_found_with_state_not_found(*exp, state, index))
				(*expanded_length)++;
		}
		else
		{
			(*expanded_length)++;
			index++;
		}
	}
}
