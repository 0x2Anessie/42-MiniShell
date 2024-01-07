#include "../../include/minishell.h"

int	determine_expansion_or_quote_removal(\
t_lexer **to_check, t_quote *state, t_expand *exp, t_data *data)
{
	int		index;

	exp->need_expand = ZERO_INIT;
	exp->quote = ZERO_INIT;
	index = ZERO_INIT;
	while (is_word_non_empty((*to_check)->word + index))
	{
		if (is_the_token_a_delimiter((*to_check)->token))
			return (NO_EXPAND);
		if (is_dollar_sign((*to_check)->word[index]))
			exp->need_expand = NEED_EXPAND;
		if (is_singl_or_doubl_quote((*to_check)->word[index]))
			exp->quote = QUOTED;
		index++;
	}
	if (is_expansion_not_required_and_quoted(exp))
		(*to_check)->word = create_cleaned_str_excluding_inactive_quots(\
		(*to_check)->word, state, data);
	else
		return (EXPANSION_REQUIRED);
	return (NO_ACTION_REQUIRED);
}

/**
 * @brief Traite chaque lexème pour une éventuelle expansion.
 *
 * @param state Pointeur vers l'état des guillemets.
 * @param data Pointeur vers la structure de données du shell.
 * @param exp Pointeur vers la structure d'expansion.
 * @param lexeme Pointeur vers le lexème actuel à traiter.
 */
void	process_lexer_for_expansion(\
t_quote *state, t_data *data, t_expand *exp, t_lexer *lexeme)
{
    t_lexer *current = lexeme;

    while (current)
	{
        if (is_dollar_or_doubl_or_singl_quote(current->word))
		{
            if (is_dollar_at_end(current->word) \
			&& determine_expansion_or_quote_removal(\
			&current, state, exp, data))
                expand_and_insert_in_lexeme_linked_list(\
				&current, state, data, exp);
        }
        reset_quoting_state(state);
        current = current->next;
    }
}

void expand(t_quote *state, char **env, t_lexer *tmp, t_data *data)
{
    t_expand *exp;

    data->full_env_var_copy_gamma = env;
    state = ft_malloc_with_tracking(data, sizeof(t_quote));
    if (!state)
        return;
    reset_quoting_state(state);
    exp = ft_malloc_with_tracking(data, sizeof(t_expand));
    if (!exp)
        return;
    process_lexer_for_expansion(state, data, exp, tmp);
}

