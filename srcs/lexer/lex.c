#include "../../include/minishell.h"

int	ft_write_fd(char *str, int fd)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
		i++;
	if (write (fd, str, i) == FAIL)
	{
		ft_printf(ERR_WRIT_NO_SPAC_LEFT_DEVC);
		g_signal_received = 1;
		return (1);
	}
	return (0);
}

void	add_lexer_to_end(t_data *data, char *str)
{
	t_lexer	*new;
	t_lexer	*current;

	new = create_new_lexer(data, str);
	if (data->lexer_list == NULL)
	{
		data->lexer_list = new;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		current = data->lexer_list;
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
		new->next = NULL;
	}
	return ;
}

void	process_lexer_input(char *str, int *i, int *j, t_quote *state)
{
	while (str[*i])
	{
		update_quoting_state(str[*i], state);
		if (!is_white_space(str[*i])
			&& (!state->doubl_quot_status && !state->singl_quot_status))
		{
			(*j)++;
			(*i)++;
		}
		else if (is_white_space(str[*i])
			&& (!state->doubl_quot_status && !state->singl_quot_status))
			break ;
		else if (state->doubl_quot_status || state->singl_quot_status)
		{
			(*j)++;
			(*i)++;
		}
	}
}
