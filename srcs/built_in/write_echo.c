
#include "../../include/minishell.h"

void	write_echo(char **tab, int i, t_data *data)
{
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "-n"))
		{
			ft_write_fd(tab[i], data->utils->node->out);
			if (tab[i + 1])
				ft_write_fd(" ", data->utils->node->out);
		}
		i++;
	}
}

int	procc_in_echo(char **tab, int i, int j, t_data *data)
{
	while (tab[i])
	{
		if (j)
		{
			if (ft_write_fd(" ", data->utils->node->out))
			{
				globi = 1;
				return (-1);
			}
			ft_write_fd(tab[i++], data->utils->node->out);
		}
		else
		{
			if (ft_write_fd(tab[i++], data->utils->node->out))
			{
				globi = 1;
				return (-1);
			}
			j++;
		}
	}
	return (0);
}

void	child_of_chill(t_data *data, int *fd, int count, t_exec utils)
{
	handle_in(utils, count);
	handle_out(utils, fd, count);
	if (is_built_in(data->lexer_list))
	{
		ft_exec_single_built_in(data->lexer_list, fd, data);
		ft_exit_child(data->utils, fd, data);
	}
	if (!slashinlex(data->lexer_list))
		exec_chemin(data);
	else if (!check_path_exec(data->lexer_list, utils, data))
	{
		if (execve(get_cmd_path(data->lexer_list->word, utils.env_lst, data), \
		get_arg(data), utils.env) == -1)
			perror(data->lexer_list->word);
	}
	globi = ERR_CODE_CMD_NOT_FOUND;
	ft_exit_child(data->utils, fd, data);
}

int	slashinlex(t_lexer *lexer)
{
	int		i;

	i = 0;
	while (lexer->word[i])
		if (lexer->word[i++] == '/')
			return (0);
	return (1);
}

void	exec_chemin(t_data *data)
{
	if (execve(data->lexer_list->word, \
		get_arg(data), data->utils->env) == -1)
		perror(data->lexer_list->word);
}
