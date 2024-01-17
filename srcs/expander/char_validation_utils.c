#include "../../include/minishell.h"

bool	is_next_char_valid_and_not_single_quote(const char *word, int *indx)
{
	return (word[++(*indx)] != '\0' && word[*indx] != '\'');
}

bool	is_not_end_of_string_or_dollar_sign(char char_to_check)
{
	return (char_to_check != '\0' && char_to_check != '$');
}

bool	is_current_char_question_mark(const char *word, int index)
{
	return (word[index] == '?');
}

bool	is_error_code_char_present(t_data *data, int indx)
{
	return (data->utils->g_signal_in_char_format[indx]);
}