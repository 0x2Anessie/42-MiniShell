/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_condtion.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:23 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:21:25 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_pid_array_null(pid_t *pid)
{
	return (pid == NULL);
}

bool	is_process_pid_valid_for_wait(pid_t pid)
{
	return (pid > 0);
}

bool	is_child_process_exited_cleanly(int status)
{
	return (WIFEXITED(status));
}

bool	should_continue_waiting_for_child_processes(int nb_node, t_data *data)
{
	return (nb_node > 0 \
	&& data->utils->heredoc_ctrl_c_uninterrupted \
	&& data->utils->nb_cmd_in_lst);
}
