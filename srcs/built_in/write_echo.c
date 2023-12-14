
#include "../../include/minishell.h"

/*
	ecri dans la sortie les argument apres verif qu'il soit pas un -n
	puis ecrit un espace
	entre les argument
	elle est utiliser quand echo doit etre afficher ailleurs que dans
	la sorti standard
*/

void	write_echo_fd(char **tab, int i)
{
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "-n"))
		{
			ft_write_fd(tab[i], g_all.utils->node->out);
			if (tab[i + 1])
				ft_write_fd(" ", g_all.utils->node->out);
		}
		i++;
	}
}

/*
	elle est apeller dans process_echo si il y a besoin d'use une sorti special
	si j n'est pas nul la fonction va ecrire un space pour separer les argument
	et n'ecrit rien si j est nul
	procc_in_echo_fd ecrit les argument de echo si il y a besoin d'une redirection
*/

int	procc_in_echo_fd(char **tab, int i, int j)
{
	while (tab[i])
	{
		if (j)
		{
			if (ft_write_fd(" ", g_all.utils->node->out))
			{
				g_all.utils->err = 1;
				return (-1);
			}
			ft_write_fd(tab[i++], g_all.utils->node->out);
		}
		else
		{
			if (ft_write_fd(tab[i++], g_all.utils->node->out))
			{
				g_all.utils->err = 1;
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
		ft_exit_child(g_all.utils, fd, data);
	}
	if (!slashinlex(data->lexer_list))
		exec_chemin(data);
	else if (!check_path_exec(data->lexer_list, utils, data))
	{
		if (execve(get_cmd_path(data->lexer_list->word, utils.env_lst, data), \
		get_arg(data), utils.env) == -1)
			perror(data->lexer_list->word);
	}
	g_all.utils->err = ERR_CODE_CMD_NOT_FOUND;
	ft_exit_child(g_all.utils, fd, data);
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
		get_arg(data), g_all.utils->env) == -1)
		perror(data->lexer_list->word);
}
