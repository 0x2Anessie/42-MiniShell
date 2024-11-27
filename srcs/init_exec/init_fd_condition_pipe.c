#include "../../include/minishell.h"

bool	is_current_token_pipe(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token == PIPE);
}

bool	is_current_token_not_pipe(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token != PIPE);
}
