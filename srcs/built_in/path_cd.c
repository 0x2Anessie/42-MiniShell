/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:40 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:41 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	l'objectif de cette fonction est de mettre a jour la valeur de la variable
	d'env OLDPWD
	on verifi d'abord si l'element actuel est bien la variable OLDPWD
	si oui s1 est mis a jour avec la nouvelle valeur de OLDPWD est retourne 1
	sinon 0
*/
int	verif_oldpwd_export(char *str, t_data *data)
{
	t_export	*tmp;
	char		*s1;

	tmp = data->utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_var_name_and_value, ENV_SET_OLDPWD, \
		ft_str_len_until_equal(ENV_SET_OLDPWD)) == 0)
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_SET_OLDPWD, str, data);
			tmp->env_var_name_and_value = create_new_var(s1, data);
			tmp->env_var_name_and_value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next_env_var_name_and_value;
	}
	return (0);
}

// la meme chose pour pwd
int	verif_pwd_export(char *str, t_data *data)
{
	t_export	*tmp;
	char		*s1;

	tmp = data->utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_var_name_and_value, \
		ENV_SET_CURRENT_WORKING_DIR, ft_str_len_until_equal(\
		ENV_SET_CURRENT_WORKING_DIR)) == 0)
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_SET_CURRENT_WORKING_DIR, str, data);
			tmp->env_var_name_and_value = create_new_var(s1, data);
			tmp->env_var_name_and_value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next_env_var_name_and_value;
	}
	return (0);
}
