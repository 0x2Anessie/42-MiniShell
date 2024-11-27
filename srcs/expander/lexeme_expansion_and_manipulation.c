#include "../../include/minishell.h"

char	*allocate_memory_for_expanded_word(\
t_lexer **exp, t_quote *st, char **env, t_data *data)
{
	int		len;
	char	*str;

	env = NULL;
	len = ZERO_INIT;
	st->var_env_match_confirmed = ZERO_INIT;
	if (is_dollar_char_then_end_of_string(*exp))
		len++;
	length_of_env_variable_value(exp, st, data, &len);
	str = malloc_track(data, sizeof(char) * (len + sizeof('\0')));
	if (!str)
		return (NULL);
	return (str);
}

void	expand_and_insert_in_lexeme_linked_list(\
t_lexer **expnd, t_quote *st, t_data *data, t_expand *exp)
{
	char	**expanded;

	initialize_expansion_parameters(exp, data, expnd, st);
	if (!exp->value_of_expanded_var_from_env)
	{
		printf(ERR_MEMORY_ALLOCATION_FAILURE);
		return ;
	}
	reset_quote_states(st);
	expand_variables_and_handle_special_chars(expnd, st, exp, data);
	exp->value_of_expanded_var_from_env[\
	exp->length_of_expanded_var_value] = '\0';
	if (is_expansion_required_and_unquoted(exp))
	{
		expanded = split_word_by_quotes(\
		data, exp->value_of_expanded_var_from_env, st);
	}
	else
	{
		(*expnd)->cmd_segment = \
		create_cleaned_str_excluding_inactive_quots(exp->\
		value_of_expanded_var_from_env, st, data);
		return ;
	}
	replace_and_extend_chained_list_lexeme(expnd, expanded, data);
}

void	insert_additional_lexemes(\
char **insert, t_lexer *back, t_data *data, int len)
{
	int		i;
	t_lexer	*tmp;

	i = ZERO_INIT;
	while (is_there_remaining_elements(++i, len))
	{
		tmp = malloc_track(data, sizeof(t_lexer) * (1));
		if (!tmp)
			return ;
		if (back)
			back->next = tmp;
		if (data->lexer_list->next)
			data->lexer_list->next->prev = tmp;
		tmp->prev = back;
		tmp->next = data->lexer_list->next;
		tmp->token = ARG;
		tmp->cmd_segment = ft_strdup(data, insert[i]);
		back = tmp;
	}
}

void	replace_and_extend_chained_list_lexeme(\
t_lexer **old_node, char **to_insert, t_data *data)
{
	int		len;
	t_lexer	*back;

	len = count_string_array_elements(to_insert);
	back = (*old_node)->prev;
	(*old_node)->cmd_segment = ft_strdup(data, to_insert[0]);
	if (len > 1)
	{
		back = (*old_node);
		insert_additional_lexemes(to_insert, back, data, len);
	}
}

char	*copy_string_excluding_quotes(\
char *original_cmd_with_quotes, \
char *cmd_without_quotes_ready_for_expand, t_quote *state)
{
	int	i;
	int	j;

	i = -1;
	j = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	while (original_cmd_with_quotes[++i])
	{
		if (is_double_quote_with_closed_single_quotes(\
		original_cmd_with_quotes[i], state))
		{
			update_quoting_state(original_cmd_with_quotes[i], state);
			continue ;
		}
		if (is_single_quote_with_closed_double_quotes(\
		original_cmd_with_quotes[i], state))
		{
			update_quoting_state(original_cmd_with_quotes[i], state);
			continue ;
		}
		cmd_without_quotes_ready_for_expand[j++] = original_cmd_with_quotes[i];
	}
	cmd_without_quotes_ready_for_expand[j] = '\0';
	return (cmd_without_quotes_ready_for_expand);
}
