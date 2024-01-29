/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:27 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:16:28 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	execute les built_in
*/
void	ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data)
{
	if (is_cmd_match(lexer_lst, CMD_EXPORT_VARS, strlen(CMD_EXPORT_VARS)))
		export_things(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_PRINT_DIRCT, strlen(CMD_PRINT_DIRCT)))
		get_pwd(lexer_lst->cmd_segment, data);
	else if (is_cmd_match(lexer_lst, CMD_ECHO, strlen(CMD_ECHO)))
		init_echo(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_CHANG_DIRCT, strlen(CMD_CHANG_DIRCT)))
		get_cd(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_ENV_VARS, strlen(CMD_ENV_VARS)))
		get_unset(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_UNSET_VARS, strlen(CMD_UNSET_VARS)))
		get_unset(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_EXIT_SHELL, strlen(CMD_EXIT_SHELL)))
		ft_exit(lexer_lst, fd, data);
	if (data->utils->node->output_fd > 0)
		close(data->utils->node->output_fd);
	lexer_lst = data->utils->head_lexer_lst;
}

void	close_fds_if_needed(int *fd, int previous_fd)
{
	if (is_fd_valid_for_close(fd[1]))
		close(fd[1]);
	if (is_fd_valid_for_close(previous_fd))
		close(previous_fd);
}

/*
	coordonne l'exec des commandes dans le bonne ordre 
*/
int	manage_exec_linked_cmd_sequence(int *fd, pid_t *pid, t_data *data, int *y)
{
	data->lexer_list = data->utils->head_lexer_lst;
	while (should_continue_execution(data, y))
	{
		data->lexer_list = find_next_command_in_lexer(data->lexer_list);
		data->utils->previous_fd = fd[0];
		if (is_pipe_creation_failed(\
		fd, data->utils->nb_cmd_in_lst))
			return (0);
		if (check_redirection_validity_in_node(data->utils->node))
		{
			if (is_single_builtin_command(data))
				ft_exec_single_built_in(data->lexer_list, fd, data);
			else
				pid[y[0]++] = create_and_execute_child_process(\
				data, fd, y[1], *(data->utils));
		}
		data->lexer_list = reaches_next_cmd_preceded_by_pipe(data->lexer_list);
		close_fds_if_needed(fd, data->utils->previous_fd);
		data->utils->node = data->utils->node->next;
	}
	closes_ends_of_pipe(fd);
	return (1);
}

/*
	Prépare et exécute les commandes, gère les processus enfants
	et ferme les descripteurs de fichier ouverts
*/
void	manage_execution_resources(t_data *data)
{
	int		wstatus;
	pid_t	*pid;
	int		fd[2];
	int		y[2];

	init_fd_and_status_vars(fd, y, &wstatus);
	pid = malloc_track(data, sizeof(pid_t) * (data->utils->cmd_nb_pipe));
	if (!pid)
		return ;
	initialize_pid_array_to_zero(pid, data->utils->cmd_nb_pipe);
	handle_process_signal();
	if (!manage_exec_linked_cmd_sequence(fd, pid, data, y))
		perror("Pipe ");
	if (!is_process_pid_valid_for_wait(*pid))
		return ;
	y[0] = 0;
	while (--data->utils->cmd_nb_pipe >= 0 && \
	data->utils->heredoc_ctrl_c_uninterrupted && data->utils->nb_cmd_in_lst)
	{
		if (pid && pid[y[0]])
			waitpid(pid[y[0]++], &wstatus, 0);
		if (WIFEXITED(wstatus))
			g_globi = WEXITSTATUS(wstatus);
	}
	close_file_descriptors_in_linked_list(data);
}
