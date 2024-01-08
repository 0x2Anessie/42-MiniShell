#include "../../include/minishell.h"

int	is_next_char_end_or_special(char *word, t_quote *state)
{
	return (word[FORWARD_OFFSET] == '\0' \
	|| is_special_syntax_character(word[FORWARD_OFFSET], state));
}

int	is_next_char_decimal_digit(char *word)
{
	return (word[FORWARD_OFFSET] && is_decimal_digit(word[FORWARD_OFFSET]));
}

int	is_special_char_found_with_state_not_found(\
t_lexer *exp, t_quote *state, int index)
{
	return (state->found == 0 && exp->word[index] \
	&& is_special_syntax_character(exp->word[index], state));
}

int	is_next_char_question_mark(t_lexer *exp, int index)
{
	return (exp->word[index + FORWARD_OFFSET] \
	&& exp->word[index + FORWARD_OFFSET] == '?');
}
