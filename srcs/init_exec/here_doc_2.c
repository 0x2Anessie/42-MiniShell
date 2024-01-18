#include "../../include/minishell.h"

void	write_line_to_heredoc(char *line, int heredoc_fd)
{
	ft_write_fd(line, heredoc_fd);
	ft_write_fd("\n", heredoc_fd);
}

char	*get_variable_value(char *var_name, t_data *data)
{
	int		i;
	char	*env_entry;
	char	*separator;
	int		name_length;

	i = 0;
	if (!var_name || !data)
		return (NULL);
	while (data->full_env_var_copy_alpha[i] != NULL)
	{
		env_entry = data->full_env_var_copy_alpha[i];
		separator = ft_strchr(env_entry, '=');
		if (!separator)
			continue ;
		name_length = separator - env_entry;
		if (ft_strncmp(var_name, env_entry, name_length) == 0 && \
			strlen2(var_name) == name_length)
			return (separator + 1);
		i++;
	}
	return (NULL);
}

int	is_backslash_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '\\')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
}

void	remove_escape_character(char **line, int index)
{
	int	i;

	i = index;
	while ((*line)[i])
	{
		(*line)[i] = (*line)[i + 1];
		i++;
	}
	(*line)[i] = '\0';
}

/**
 * Vérifie si le caractère à l'index donné doit être échappé.
 */
int	is_escaped(char *line, int index)
{
	return (index > 0 && line[index - 1] == '\\');
}
