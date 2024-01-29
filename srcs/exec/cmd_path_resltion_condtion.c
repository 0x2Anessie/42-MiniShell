/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_resltion_condtion.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:15 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:23:16 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_env_var_path_name(t_env *env)
{
	return (!ft_strncmp(\
	env->var_env_name_and_value, ENV_SET_PATH_PREFIX, \
	ft_strlen(ENV_SET_PATH_PREFIX)));
}

bool	is_invalid_direct_path(const char *cmd)
{
	return (access(cmd, X_OK) && *cmd == '/');
}
