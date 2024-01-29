/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:05 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:22:58 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirect_input_error(t_node *node, t_lexer *lexer_lst)
{
	if (is_first_input_redirection_error(node))
	{
		node->input_fd = INPUT_FD_REDIRECTION_FAIL;
		perror(lexer_lst->next->cmd_segment);
	}
	else
	{
		node->input_fd = INPUT_FD_REDIRECTION_FAIL;
		ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
	}
}

char	*ft_strdup(t_data *data, char *src)
{
	int		index;
	char	*dest;

	index = ZERO_INIT;
	if (!src)
		return (NULL);
	while (src[index])
		index++;
	dest = malloc_track(data, sizeof(char) * index + sizeof('\0'));
	if (!dest)
		return (NULL);
	index = 0;
	while (src[index])
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}

int	is_token_type_cmd(t_lexer *lexer_lst)
{
	while (lexer_lst)
	{
		if (is_current_lexer_token_cmd(lexer_lst))
			return (CMD_FOUND);
		lexer_lst = lexer_lst->next;
	}
	return (CMD_NOT_FOUND);
}

int	count_cmd_in_lexer_linked_list(t_lexer *lexer_list)
{
	int	cmd_count;

	cmd_count = ZERO_INIT;
	while (lexer_list)
	{
		if (is_current_lexer_token_cmd(lexer_list))
			cmd_count++;
		lexer_list = lexer_list->next;
	}
	return (cmd_count);
}

int	count_pipe_or_end_in_lexer_linked_list(t_lexer *lexer_list)
{
	int	index;

	index = ZERO_INIT;
	while (lexer_list)
	{
		if (is_pipe_or_end_of_lexer_linked_list(lexer_list))
			index++;
		lexer_list = lexer_list->next;
	}
	return (index);
}
