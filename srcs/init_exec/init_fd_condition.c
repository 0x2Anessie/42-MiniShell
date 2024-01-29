/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_condition.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polo <polo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:28 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 01:30:29 by polo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_input_fd_open_ready_for_read(t_node *node)
{
	return (node->input_fd > 0);
}

bool	is_append_out_followed_by_fd_token(t_lexer *lex_lst)
{
	return (lex_lst && lex_lst->next \
	&& lex_lst->token == APPEND_OUT \
	&& lex_lst->next->token == FD);
}

/**
	Vérifie si le descripteur de fichier de sortie est ouvert et prêt à être
 	fermé.
 */
bool	is_output_fd_open_for_closing(t_node *node)
{
	return (node->output_fd > 0);
}
