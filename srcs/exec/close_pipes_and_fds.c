/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_pipes_and_fds.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:06 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:05:15 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	ferme le pipe
*/
int	closes_ends_of_pipe(int *fd)
{
	if (fd[PIPE_READ_END] > 0)
		close(fd[PIPE_READ_END]);
	if (fd[PIPE_WRITE_END] > 0)
		close(fd[PIPE_WRITE_END]);
	return (1);
}

/*
	ferme les fd d'entrer et de sortie pour chaque noeud
*/
int	close_file_descriptors_in_linked_list(t_data *data)
{
	data->utils->node = data->utils->head_cmd_lst;
	while (data->utils->node)
	{
		if (data->utils->node->input_fd > 1)
			close(data->utils->node->input_fd);
		if (data->utils->node->output_fd > 1)
			close(data->utils->node->output_fd);
		data->utils->node = data->utils->node->next;
	}
	return (1);
}
