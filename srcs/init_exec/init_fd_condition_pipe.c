/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_condition_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:25:24 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:25:25 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_current_token_pipe(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token == PIPE);
}

bool	is_current_token_not_pipe(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token != PIPE);
}
