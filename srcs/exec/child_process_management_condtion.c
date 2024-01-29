/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_management_condtion.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:58 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:59 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_execve_failed(char *command_path, char **arguments, char **env)
{
	return (execve(command_path, arguments, env) == FAIL);
}
