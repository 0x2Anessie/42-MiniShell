/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word_by_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:24:33 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:55:04 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_subwords_with_quote_states(char *word, t_quote *state)
{
	int		index;
	int		j;

	index = ZERO_INIT;
	j = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	while (word[index])
	{
		update_quoting_state(word[index], state);
		if (is_start_word_considering_quotes(word, index, state))
			j++;
		index++;
	}
	return (j);
}

char	**split_word_by_quotes(t_data *data, char *word, t_quote *state)
{
	int		index;
	int		j;
	char	**arr;

	arr = \
	malloc_track(\
	data, sizeof(char *) * (count_subwords_with_quote_states(\
	word, state) + sizeof('\0')));
	if (!arr)
		return (NULL);
	index = ZERO_INIT;
	j = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	while (word[index])
	{
		update_quoting_state(word[index], state);
		if (is_start_word_considering_quotes(word, index, state))
			arr[j++] = \
			alloc_copy_word_excluding_quot_with_tracking(\
			&word[index], state, data);
		index++;
	}
	arr[j] = NULL;
	return (arr);
}
