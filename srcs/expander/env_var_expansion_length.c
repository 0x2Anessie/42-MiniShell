/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion_length.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:22:43 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:41:51 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	calcule la len necessaire pour une variable d'env
*/
int	get_env_var_expansion_length(\
char *word, int *length, char *env_var, t_quote *state)
{
	int			i;
	int			y;

	i = ZERO_INIT;
	y = ZERO_INIT;
	while (word[i++] && env_var[y] \
	&& env_var[y] != '=' \
	&& word[i] == env_var[y])
		y++;
	if ((word[i] == '\0' \
	|| is_special_syntax_character(\
	word[i], state) || word[i] == '$') && env_var[y]
		&& y != 0 && env_var[y] == '=')
	{
		state->var_env_match_confirmed = 1;
		while (env_var[++y])
			(*length)++;
		return (i);
	}
	return (i);
}

int	calculate_length_until_single_quote(char *word, int *length)
{
	int		index;

	index = ZERO_INIT;
	while (word[index] && word[index] != '\'')
	{
		(*length)++;
		index++;
	}
	return (index);
}

int	process_env_var_expansion_if_match(\
char *word, char **env_var, t_quote *state, int *length)
{
	int	word_index;

	word_index = 1;
	if (is_char_matching_env_var(word, word_index, *env_var, 0))
	{
		word_index = get_env_var_expansion_length(\
		word, length, *env_var, state);
	}
	return (word_index);
}

int	calculate_length_for_env_expansion(\
char *word, char **env_var, t_quote *state, int *length)
{
	int	env_var_index;
	int	word_index;

	env_var_index = ZERO_INIT;
	state->var_env_match_confirmed = ZERO_INIT;
	if (is_only_single_quote_on(state))
		return (calculate_length_until_single_quote(word, length));
	else
	{
		while (env_var[env_var_index])
		{
			word_index = process_env_var_expansion_if_match(\
			word, &env_var[env_var_index], state, length);
			if (state->var_env_match_confirmed == 1)
				return (word_index);
			env_var_index++;
		}
	}
	return (word_index);
}

int	calculate_expanded_env_var_length(\
char *word, char **env_var, t_quote *state, int *length)
{
	state->var_env_match_confirmed = ZERO_INIT;
	if (is_only_single_quote_on(state))
		return (calculate_length_until_single_quote(word, length));
	else
	{
		return (calculate_length_for_env_expansion(\
		word, env_var, state, length));
	}
}
