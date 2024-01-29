/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cmd_management_and_init_vars.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:49 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:18:55 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_lexer	*reaches_next_cmd_preceded_by_pipe(t_lexer *lexer_list)
{
	while (is_current_token_not_pipe(lexer_list))
		lexer_list = lexer_list->next;
	if (is_current_token_pipe(lexer_list))
		lexer_list = lexer_list->next;
	return (lexer_list);
}

int	count_args_until_pipe_for_cmd_array(t_lexer *lexer_list)
{
	int	index;

	index = 1;
	while (is_current_token_not_pipe(lexer_list))
	{
		if (is_current_token_cmd_arg(lexer_list))
			index++;
		lexer_list = lexer_list->next;
	}
	return (index);
}

void	initialize_pid_array_to_zero(pid_t *pid_array, int num_nodes)
{
	while (num_nodes-- > 0)
		*(pid_array++) = 0;
}

void	init_fd_and_status_vars(int *fd, int *y, int *wstatus)
{
	*wstatus = ZERO_INIT;
	y[0] = ZERO_INIT;
	y[1] = -1;
	fd[0] = ZERO_INIT;
	fd[1] = ZERO_INIT;
}
