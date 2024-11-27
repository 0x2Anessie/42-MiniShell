#include "../../include/minishell.h"

int	is_special_syntax_character(char c, t_quote *st)
{
	(void)st;
	if (c)
	{
		if (c == ':' || c == '=' || c == '%' || c == '^' || c == '+' || c == '~'
			|| c == '/' || c == ' '
			|| ((c == '"' || c == '\''))
			|| c == ']' || c == '@' || c == ',')
			return (1);
	}
	return (0);
}

int	is_dollar_or_doubl_or_singl_quote(char *word)
{
	int	index;

	index = ZERO_INIT;
	while (word[index])
	{
		if (word[index] == '$' || word[index] == '"' || word [index] == '\'')
			return (CHAR_IS_SPECIAL);
		index++;
	}
	return (CHAR_IS_NOT_SPECIAL);
}

int	is_dollar_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '$')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
}
