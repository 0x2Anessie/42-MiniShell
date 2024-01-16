#include "../../include/minishell.h"

int	is_previous_token_redirection(t_lexer *token)
{
	return (token->prev->token == APPEND_OUT \
		|| token->prev->token == REDIRECT_IN \
		|| token->prev->token == REDIRECT_OUT \
		|| token->prev->token == REDIRECT_INOUT);
}

void	get_token_in_node(t_lexer **lexer_list, t_lexer *tmp)
{
	t_lexer	*first;

	tmp = *lexer_list;
	while (tmp)
	{
		if (tmp->segment_position == 1)
			first = tmp;
		if (tmp->segment_position == 1 && tmp->cmd_segment[0] != '<' \
		&& tmp->cmd_segment[0] != '>')
			tmp->token = CMD;
		else if (tmp->segment_position == 0)
			tmp->token = PIPE;
		else if (tmp->cmd_segment[0] == '<' || tmp->cmd_segment[0] == '>')
			tmp->token = which_redir(tmp);
		else if (is_previous_token_redirection(tmp))
			tmp->token = FD;
		else if (tmp->prev->token == HERE_DOC)
			tmp->token = DELIMITER;
		else if ((tmp->prev->token == FD || tmp->prev->token == DELIMITER)
			&& (!tmp->next || tmp->next->cmd_segment[0] == '|' || tmp->next))
			assign_command_or_argument_token(tmp, first);
		else
			tmp->token = ARG;
		tmp = tmp->next;
	}
}
