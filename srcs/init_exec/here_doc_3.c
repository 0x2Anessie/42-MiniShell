/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:24:48 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:19:15 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	manage_here_doc_process(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->is_this_an_exec_in_heredoc = TRUE;
	handle_sig(data);
	node->heredoc_tmp_fullname = HEREDOC_TEMP_FILE;
	node->here_doc_fd = open(\
	node->heredoc_tmp_fullname, heredoc_tmp_file_flags(), PERM_RWX_ALL);
	if (is_heredoc_file_opening_failed(node->here_doc_fd))
		return ;
	ft_read_input(node, lexer_lst, data);
	data->utils->is_this_an_exec_in_heredoc = FALSE;
	handle_sig(data);
	close(node->here_doc_fd);
}
