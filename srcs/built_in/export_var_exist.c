/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var_exist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:24 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:26 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	elle verifi si une variable specifier par str existe dans
	la head_of_linked_list_env_var en comparant str avec la valeur
	de chaque noeudsi une variable est trouver et que str contient
	un = l valeur de la variable est mis a jour avec str
*/
int	verif_var_exist_export(t_exec *utils, char *str, t_data *data)
{
	t_export	*tmp;

	tmp = utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (tmp->env_var_name_and_value && (\
		(ft_strncmp(tmp->env_var_name_and_value, str, \
		ft_str_len_until_equal(tmp->env_var_name_and_value))) == 0 \
		&& (ft_str_len_until_equal(str) == ft_str_len_until_equal(\
		tmp->env_var_name_and_value))))
		{
			if (verif_equal(str, '=') && (ft_str_len_until_equal(\
			str) == ft_str_len_until_equal(tmp->env_var_name_and_value)))
				tmp->env_var_name_and_value = case_egale(str, data);
			return (1);
		}
		tmp = tmp->next_env_var_name_and_value;
	}
	return (0);
}

int	verif_equal(char *str, char c)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/*
	verifie si une la variable str existe deja dans la liste des variable d'env
	(env_lst) en comparant str avec le contenu de chaque noeud
	si il trouve il met a jour la variable d'env avec str
*/
int	verif_var_exist(t_exec *utils, char *str)
{
	t_env	*tmp;

	tmp = utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (tmp->var_env_name_and_value && !(ft_strncmp
				(tmp->var_env_name_and_value, str, ft_str_len_until_equal(tmp
						->var_env_name_and_value))) && (ft_str_len_until_equal
				(str) == ft_str_len_until_equal(tmp->var_env_name_and_value)))
		{
			tmp->var_env_name_and_value = str;
			tmp = utils->linked_list_full_env_var_copy_alpha;
			return (1);
		}
		tmp = tmp->next_var_env_name_and_value;
	}
	return (0);
}

/* comme celle d'avant mais ne met pas a jour la variable et parcour
la head_of_linked_list_env_var*/
int	verif_var_exist_export_not_maj(t_exec *utils, char *str)
{
	t_export	*tmp;

	tmp = utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (tmp->env_var_name_and_value \
		&& ((ft_strncmp(tmp->env_var_name_and_value, str, \
		ft_str_len_until_equal(tmp->env_var_name_and_value))) == 0 \
		&& (ft_str_len_until_equal(str) \
		== ft_str_len_until_equal(tmp->env_var_name_and_value))))
		{
			return (1);
		}
		tmp = tmp->next_env_var_name_and_value;
	}
	return (0);
}
