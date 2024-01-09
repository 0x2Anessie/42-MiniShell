#include "../../include/minishell.h"

bool	is_expansion_required_and_unquoted(t_expand *exp)
{
    return (exp->need_expand == 1 && exp->quote == 0);
}

bool	is_there_remaining_elements(int index, int total_length)
{
    return (index < total_length);
}

bool	is_single_quote_with_closed_double_quotes(char c, t_quote *state)
{
    return (c == '\'' && is_doubl_quote_closed(state));
}

bool	is_double_quote_with_closed_single_quotes(char c, t_quote *state)
{
    return (c == '"' && is_single_quote_closed(state));
}

bool    is_dollar_char_then_end_of_string(t_lexer *exp)
{
    return (exp->word[0] == '$' && exp->word[1] == '\0');
}
