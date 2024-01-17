#include "../../include/minishell.h"

void	get_data_in_node(t_lexer **lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 1;
	tmp = *lexer_list;
	while (tmp)
	{
		if (tmp->cmd_segment[0] == '|')
			i = 0;
		tmp->segment_position = i;
		tmp = tmp->next;
		i++;
	}
}

int	get_word_in_list(char *str, int i, t_data *data, t_lexer *tmp)
{
	char	*word;
	int		j;
	int		k;
	int		x;
	t_quote	*state;

	state = ft_malloc_with_tracking(data, sizeof(t_quote));
	if (!state)
		return (0);
	word = NULL;
	x = i;
	k = 0;
	j = 0;
	reset_quoting_state(state);
	process_lexer_input(str, &i, &j, state);
	word = ft_malloc_with_tracking(data, sizeof(char) * (j + sizeof('\0')));
	if (!word)
		return (0);
	word[j] = '\0';
	while (k < j)
		word[k++] = str[x++];
	add_lexer_to_end(data, word);
	get_data_in_node(&data->lexer_list);
	get_token_in_node(&data->lexer_list, tmp);
	return (j);
}

void	process_input_string(\
t_data *data, t_lexer *tmp, t_lexer *current, int i)
{
	int	j;
	int	x;

	x = ZERO_INIT;
	while (data->minishell_input_cmd[i])
	{
		j = 0;
		while (data->minishell_input_cmd[i] == ' ' || \
		(data->minishell_input_cmd[i] >= TAB && \
		data->minishell_input_cmd[i] <= SHIFT_OUT))
			i++;
		if (!data->minishell_input_cmd || !data->minishell_input_cmd[i])
		{
			data->lexer_list = current;
			get_token_in_node(&current, tmp);
			return ;
		}
		j = get_word_in_list(data->minishell_input_cmd, i, data, tmp);
		if (x == 0)
			current = data->lexer_list;
		i = i + j;
		x++;
	}
	data->lexer_list = current;
}

void	print_lexer_content1(t_data *data)
{
	t_lexer	*current;

	current = data->lexer_list;
	while (current != NULL)
	{
		current = current->next;
	}
}

void	ft_init_lexer_process(t_data *data)
{
	int		i;
	t_lexer	*current;
	t_lexer	*tmp;

	tmp = NULL;
	current = NULL;
	data->lexer_list = NULL;
	data->input_line_word_count = \
	count_words_in_input(data->minishell_input_cmd);
	i = ZERO_INIT;
	process_input_string(data, tmp, current, i);
	print_lexer_content1(data);
}
