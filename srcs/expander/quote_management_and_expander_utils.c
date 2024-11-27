#include "../../include/minishell.h"

void	update_quoting_state(char c, t_quote *state)
{
	if (c == '\'')
	{
		if (is_single_quote_closed(state))
			state->singl_quot_status = SIMPL_QUOT_OPEN;
		else
		{
			state->singl_quot_start_status = SINGLE_QUOTE_NOT_STARTED;
			state->singl_quot_status = SIMPL_QUOT_CLOS;
		}
	}
	else if (c == '"')
	{
		if (is_doubl_quote_closed(state))
			state->doubl_quot_status = DOUBL_QUOT_OPEN;
		else
			state->doubl_quot_status = DOUBL_QUOT_CLOS;
	}
	if (is_only_single_quote_on(state))
		state->singl_quot_start_status = SINGLE_QUOTE_STARTED;
}

int	is_char_cmd_separator(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == ' ')
		return (IS_SEPARATOR);
	return (NOT_SEPARATOR);
}

bool	is_cmd_separator_and_quotes_off(char c, t_quote *state)
{
	return (is_char_cmd_separator(c) && is_both_quotes_off(state));
}

int	is_end_of_command_word(char *cmd, int i, t_quote *state)
{
	if (is_cmd_separator_and_quotes_off(cmd[i], state))
		return (TRUE);
	return (FALSE);
}

int	count_string_array_elements(char **arr)
{
	int	index;

	index = ZERO_INIT;
	while (arr[index])
		index++;
	return (index);
}
