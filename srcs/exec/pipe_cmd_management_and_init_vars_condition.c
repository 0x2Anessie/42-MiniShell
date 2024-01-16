#include "../../include/minishell.h"

bool	is_current_token_cmd_arg(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token == ARG);
}

bool	is_index_less_than_num_nodes(int index, int num_nodes)
{
	return (index < num_nodes);
}
