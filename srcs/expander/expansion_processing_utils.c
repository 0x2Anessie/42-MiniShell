/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_processing_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:22:55 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:45:31 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	append_chars_expnt_until_singl_quot(char *word, t_expand *exp)
{
	int	indx;

	indx = -1;
	while (is_next_char_valid_and_not_single_quote(word, &indx))
	{
		exp->value_of_expanded_var_from_env[\
		exp->length_of_expanded_var_value++] = word[indx];
	}
	return (indx);
}

int	append_curnt_error_code_to_expansion_struc(t_expand *exp, t_data *data)
{
	int	indx;

	indx = -1;
	while (is_error_code_char_present(data, ++indx))
		exp->value_of_expanded_var_from_env[\
		exp->length_of_expanded_var_value++] = \
		data->utils->g_signal_in_char_format[indx];
	exp->var_env_match_found = 1;
	return (2);
}

int	expand_env_vars_with_question_mark_handling(\
char *w, t_expand *exp, t_data *data, t_quote *state)
{
	int	i;
	int	env_var_char_idx;

	i = 1;
	data->env_var_line_idx = -1;
	exp->var_env_match_found = ZERO_INIT;
	while (is_valid_env_var_entry(data))
	{
		env_var_char_idx = 0;
		i = 1;
		if (is_current_char_question_mark(w, i))
			return (find_and_expand_env_var_with_special_char(\
			w, exp, data, state));
		if (is_char_matching_env_var(\
		w, i, data->full_env_var_copy_gamma[data->env_var_line_idx], \
		env_var_char_idx))
		{
			i = find_and_expand_env_var_with_special_char(w, exp, data, state);
			if (is_expansion_found(exp))
				return (i);
		}
	}
	if (is_expansion_not_found(exp))
		return (handle_unfound_expansion_word(w, state));
	return (i);
}
