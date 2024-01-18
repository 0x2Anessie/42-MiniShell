#include "../../include/minishell.h"

bool	is_word_end_or_special(const char *word, int index, t_quote *state)
{
	return (word[index] == '\0' || word[index] == '$' \
	|| is_special_syntax_character(word[index], state));
}

bool	is_expansion_not_found(t_expand *exp)
{
	return (exp->var_env_match_found == NOT_FOUND);
}

bool	is_expansion_found(t_expand *exp)
{
	return (exp->var_env_match_found == FOUND);
}
