/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_redirect_condition.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:37 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:25:39 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_redirect_out_followed_by_fd_token(t_lexer *lex_lst)
{
	return (lex_lst && lex_lst->next \
	&& lex_lst->token == REDIRECT_OUT \
	&& lex_lst->next->token == FD);
}

bool	is_input_redirection_followed_by_token_fd(t_lexer *lexer_lst)
{
	return (lexer_lst \
	&& lexer_lst->token == REDIRECT_IN \
	&& lexer_lst->next->token == FD);
}

bool	is_output_redirection_error_detected(t_node *node)
{
	return (node->is_output_redirection_feasible \
	&& node->output_fd == OUTPUT_FD_NOT_CONFIGURED \
	&& node->input_fd != INPUT_FD_REDIRECTION_FAIL \
	&& node->output_redirection_error_id != \
	OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE);
}

bool	is_normal_output_redirection_error_detected(\
t_node *node, t_lexer *lex_lst)
{
	return (node->output_redirection_error_id != \
	OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE \
	&& (node->output_fd == OUTPUT_FD_NOT_CONFIGURED \
	|| !access(lex_lst->next->cmd_segment, W_OK)));
}

bool	is_output_append_redirection_error_detected(\
t_node *node, t_lexer *lex_lst)
{
	return (node->output_redirection_error_id != \
	OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE \
	&& (node->output_fd == OUTPUT_FD_NOT_CONFIGURED \
	|| !access(lex_lst->next->cmd_segment, F_OK)));
}
