#include "../../include/minishell.h"

bool	is_current_char_dollar_sign(t_lexer *exp, int index)
{
	return (exp->cmd_segment[index] == '$');
}
