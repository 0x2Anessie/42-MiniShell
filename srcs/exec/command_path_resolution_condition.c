/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_resolution_condition.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:30 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:18:14 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_cmd_empty_or_null(char *cmd)
{
	return (cmd == NULL || cmd[0] == '\0');
}

bool	is_cmd_executable(char *command)
{
	return (access(command, X_OK) == 0);
}

bool	is_lexer_token_cmd_arg(t_lexer *lexer_item)
{
	return (lexer_item != NULL && lexer_item->token == ARG);
}

/*
 Vérifie si l'itération dans la liste des lexèmes doit continuer.
 */
bool	should_continue_lexer_iteration(\
t_lexer *lexer_item, int current_index, int total_args)
{
	return (lexer_item->next != NULL && current_index < total_args);
}

bool	is_env_list_empty_or_null(t_env *env_lst)
{
	return (env_lst == NULL || env_lst->var_env_name_and_value == NULL);
}
