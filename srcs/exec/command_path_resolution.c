/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_resolution.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:21:34 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 19:20:51 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**extract_paths_from_env(t_env *env_lst, t_data *data)
{
	char	**path;

	path = NULL;
	if (is_env_list_empty_or_null(env_lst))
	{
		return (NULL);
	}
	while (env_lst)
	{
		if (is_env_var_path_name(env_lst))
		{
			path = ft_split_mini(env_lst->var_env_name_and_value \
			+ ft_strlen(ENV_SET_PATH_PREFIX), ':', data);
			return (path);
		}
		env_lst = env_lst->next_var_env_name_and_value;
	}
	return (path);
}

/*
	retrouve le chemin d'une commande apres le PATH
*/
char	*find_command_full_path(char *cmd, t_env *env_lst, t_data *data)
{
	int		index;
	char	*tmp;
	char	**path;

	index = ZERO_INIT;
	if (is_cmd_empty_or_null(cmd))
		return (NULL);
	if (is_cmd_executable(cmd))
		return (cmd);
	else if (is_invalid_direct_path(cmd))
		return (NULL);
	path = extract_paths_from_env(env_lst, data);
	if (path)
	{
		while (path && path[index])
		{
			tmp = ft_strjoin2_mini(path[index], "/", data);
			tmp = ft_strjoin2_mini(tmp, cmd, data);
			if (!access(tmp, F_OK))
				return (tmp);
			index++;
		}
	}
	return (NULL);
}

/*
	verifie si le chemin absolu d'une commande est bon
	on utilise find_command_full_path pour voir si elle existe
*/
int	check_if_cmd_full_path_exec_is_valid(\
t_lexer *lexer, t_exec utils, t_data *data)
{
	if (!find_command_full_path(\
	lexer->cmd_segment, utils.linked_list_full_env_var_copy_alpha, data))
	{
		write(STDERR_FILENO, lexer->cmd_segment, strlen2(lexer->cmd_segment));
		write(\
		STDERR_FILENO, ERR_MSG_CMD_NOT_FOUND, \
		ft_strlen(ERR_MSG_CMD_NOT_FOUND));
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

char	**build_arg_array_from_lexer(t_data *data)
{
	char	**arg;
	int		nb_arg;
	int		index;

	index = ZERO_INIT;
	nb_arg = count_args_until_pipe_for_cmd_array(data->lexer_list);
	arg = malloc_track(data, sizeof(char *) * (nb_arg + 1));
	arg[index++] = data->lexer_list->cmd_segment;
	while (should_continue_lexer_iteration(data->lexer_list, index, nb_arg))
	{
		if (is_lexer_token_cmd_arg(data->lexer_list->next))
			arg[index++] = data->lexer_list->next->cmd_segment;
		data->lexer_list->next = data->lexer_list->next->next;
	}
	arg[index] = NULL;
	return (arg);
}
