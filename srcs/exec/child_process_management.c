/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_management.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:02 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 18:34:08 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	cherche si il y a un "/" au qu'elle cas c'est un chemain absolu
*/
int	check_for_slash_path_delimiter(t_lexer *lexer)
{
	int		indx;

	indx = ZERO_INIT;
	while (lexer->cmd_segment[indx])
	{
		if (lexer->cmd_segment[indx++] == '/')
			return (SLASH_FOUND);
	}
	return (NO_SLASH_FOUND);
}

/*
	execute une commande avec ces arguments
	construit un tab d'arg a part de la lexer_lst et appel
	is_execve_failed pour savoir si l'exec c bien passer
*/
void	execute_lexer_command_with_args(t_data *data)
{
	char	**arguments;

	arguments = build_arg_array_from_lexer(data);
	if (is_execve_failed(data->lexer_list->cmd_segment, arguments, \
	data->utils->full_env_var_copy_beta))
		perror(data->lexer_list->cmd_segment);
}

/*
	execute des commandes en gerant les redirection et commande integrer
	elle configure les redirection d'entree et de sortie
	puis execute direct si c un built_in ou utilise execve
	pour les autres
*/
void	exec_cmd_with_redirection_and_builtins(\
t_data *data, int *fd, int count, t_exec utils)
{
	char	*command_full_path;
	char	**arguments;

	configure_stdin_redirection_for_command(utils, count);
	configure_stdout_redirection_for_command(utils, fd, count);
	if (is_built_in_command(data->lexer_list))
	{
		ft_exec_single_built_in(data->lexer_list, fd, data);
		ft_exit_child(fd, data);
	}
	if (!check_for_slash_path_delimiter(data->lexer_list))
		execute_lexer_command_with_args(data);
	else if (!check_if_cmd_full_path_exec_is_valid(\
	data->lexer_list, utils, data))
	{
		command_full_path = find_command_full_path(data->lexer_list->\
		cmd_segment, utils.linked_list_full_env_var_copy_alpha, data);
		arguments = build_arg_array_from_lexer(data);
		if (is_execve_failed(\
		command_full_path, arguments, utils.full_env_var_copy_beta))
			perror(data->lexer_list->cmd_segment);
	}
	g_globi = 127;
	ft_exit_child(fd, data);
}

/*
	cree un procesus enfant pour executer une commande
	si fork reussit appel exec_cmd_with_redirection_and_builtin
	pour executer la commande
*/
pid_t	create_and_execute_child_process(\
t_data *data, int *fd, int count, t_exec utils)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork");
		return (FT_FAILURE_EXIT);
	}
	if (pid == CHILD_PROCESS)
		exec_cmd_with_redirection_and_builtins(data, fd, count, utils);
	return (pid);
}
