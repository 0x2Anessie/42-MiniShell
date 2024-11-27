#include "../../include/minishell.h"

bool	is_fd_valid_for_close(int fd)
{
	return (fd > 0);
}

bool	is_fd_set_for_redirection(int fd)
{
	return (fd >= 0);
}

/*
	onfigure les redirections d'entrer standard pour une commande
*/
void	configure_stdin_redirection_for_command(t_exec utils, int count)
{
	if (is_fd_set_for_redirection(utils.node->input_fd))
	{
		dup2(utils.node->input_fd, STDIN_FILENO);
		if (is_fd_valid_for_close(utils.node->input_fd))
			close(utils.node->input_fd);
		if (utils.previous_fd)
			close(utils.previous_fd);
	}
	else if (count)
	{
		dup2(utils.previous_fd, STDIN_FILENO);
		if (is_fd_valid_for_close(utils.previous_fd))
			close(utils.previous_fd);
	}
}

bool	is_not_last_command_and_fd_open(int count, int total_cmds, int fd)
{
	return (count != total_cmds - 1 && fd > 0);
}

/*
	configure les redirections de sortie standard pour une commande
*/
void	configure_stdout_redirection_for_command(\
t_exec utils, int *fd, int count)
{
	if (is_fd_valid_for_close(utils.node->output_fd))
	{
		dup2(utils.node->output_fd, STDOUT_FILENO);
		if (is_fd_valid_for_close(utils.node->input_fd))
			close (utils.node->input_fd);
	}
	else if (is_not_last_command_and_fd_open(\
	count, utils.nb_cmd_in_lst, fd[1]))
		dup2(fd[1], STDOUT_FILENO);
	if (is_fd_valid_for_close(fd[1]))
		close(fd[1]);
	if (is_fd_valid_for_close(fd[0]))
		close (fd[0]);
}
