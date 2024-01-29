/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_heredoc_condition.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:32 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:30:57 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_here_doc_followed_by_delimiter(t_lexer *lexer_lst)
{
	return (lexer_lst \
	&& lexer_lst->token == HERE_DOC \
	&& lexer_lst->next \
	&& lexer_lst->next->token == DELIMITER);
}

bool	is_heredoc_tmp_file_exists(t_node *node)
{
	return (!access(node->heredoc_tmp_fullname, F_OK));
}

void	configure_here_doc_input(t_node *node, t_lexer *lex_lst, t_data *data)
{
	if (is_here_doc_followed_by_delimiter(lex_lst))
	{
		if (is_input_fd_open_ready_for_read(node))
			close (node->input_fd);
		manage_here_doc_process(node, lex_lst, data);
		if (is_heredoc_tmp_file_exists(node))
		{
			node->input_fd = open(node->heredoc_tmp_fullname, O_RDONLY);
			unlink(node->heredoc_tmp_fullname);
		}
		else
		{
			node->input_fd = INPUT_FD_REDIRECTION_FAIL;
			unlink(node->heredoc_tmp_fullname);
			perror(node->heredoc_tmp_fullname);
		}
	}
}
