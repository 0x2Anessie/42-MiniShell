#include "../../include/minishell.h"

char	*extract_and_get_var_value(char **line, int var_name_start,
int var_name_length, t_data *data)
{
	char	*var_name;
	char	*value;

	var_name = malloc(sizeof(char *) * (var_name_length + 1));
	if (!var_name)
		return (NULL);
	strncpy(var_name, *line + var_name_start, var_name_length);
	var_name[var_name_length] = '\0';
	value = get_variable_value(var_name, data);
	free (var_name);
	return (value);
}

int	get_var_name_length(char *line, int start)
{
	int	length;

	length = 0;
	while (line[start + length] && is_valid_variable_char(line[start + length]))
		length++;
	return (length);
}

void	expand_variable(char **line, int index, t_data *data)
{
	int		var_name_start;
	int		var_name_length;
	int		new_line_length;
	char	*new_line;
	char	*var_value;

	var_name_start = index + 1;
	var_name_length = get_var_name_length(*line, var_name_start);
	if (var_name_length == 0)
		return ;
	var_value = extract_and_get_var_value(line, var_name_start,
			var_name_length, data);
	if (!var_value)
		var_value = "";
	new_line_length = strlen2(*line) - var_name_length - 1 + strlen2(var_value);
	new_line = malloc(new_line_length + 1);
	if (!new_line)
		return ;
	ft_strncpy(new_line, *line, index);
	ft_strcpy(new_line + index, var_value);
	ft_strcpy(new_line + index + strlen2(var_value), *line + var_name_start
		+ var_name_length);
	free (*line);
	*line = new_line;
}

/*
	Traite une ligne lue dans un heredoc pour l'expansion des variables et
	la gestion des échappements.
*/
void	process_heredoc_line(char **line, t_data *data)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$' && !is_escaped(*line, i))
			expand_variable(line, i, data);
		else if (is_escaped(*line, i))
			remove_escape_character(line, i);
		i++;
	}
}

void	ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	char	*line;

	data->utils->stdin_fd_for_heredoc = dup(0);
	while (INFINITY_LOOP)
	{
		line = readline("> ");
		if (line == NULL && data->utils->heredoc_ctrl_c_uninterrupted)
		{
			dup2(data->utils->stdin_fd_for_heredoc, STDIN_FILENO);
			break ;
		}
		if (is_heredoc_delimiter_matched(lexer_lst->next->cmd_segment, line))
		{
			free(line);
			break ;
		}
		process_heredoc_line(&line, data);
		write_line_to_heredoc(line, node->here_doc_fd);
		free(line);
	}
	close(data->utils->stdin_fd_for_heredoc);
	free(data->utils->heredoc_input_buffer);
}
