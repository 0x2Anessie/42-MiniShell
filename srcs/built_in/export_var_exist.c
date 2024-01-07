
#include "../../include/minishell.h"

/*
	elle verifi si une variable specifier par str existe dans la head_of_linked_list_env_var en
	comparant str avec la valeur de chaque noeud
	si une variable est trouver et que str contient un = l valeur de la variable
	est mis a jour avec str
*/
int	verif_var_exist_export(t_exec *utils, char *str, t_data *data)
{
	t_export	*tmp;

	tmp = utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (tmp->value && ((ft_strncmp(tmp->value, str, \
			ft_str_len_until_equal(tmp->value))) == 0 \
				&& (ft_str_len_until_equal(str) == ft_str_len_until_equal(tmp->value))))/*         ---> condition non intelligible --> fonction         */
		{
			if (verif_equal(str, '=')
				&& (ft_str_len_until_equal(str) == ft_str_len_until_equal(tmp->value)))
			tmp->value = case_egale(str, data);
			return (1);
		}
		tmp = tmp->next;
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
		if (tmp->content && !(ft_strncmp
				(tmp->content, str, ft_str_len_until_equal(tmp
						->content))) && (ft_str_len_until_equal
				(str) == ft_str_len_until_equal(tmp->content)))/*         ---> condition non intelligible --> fonction         */
		{
			tmp->content = str;
			tmp = utils->linked_list_full_env_var_copy_alpha;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

// comme celle d'avant mais ne met pas a jour la variable et parcour la head_of_linked_list_env_var
int	verif_var_exist_export_not_maj(t_exec *utils, char *str)
{
	t_export	*tmp;

	tmp = utils->head_of_linked_list_env_var;
	while (tmp)
	{
		if (tmp->value && ((ft_strncmp(tmp->value, str, \
		ft_str_len_until_equal(tmp->value))) == 0 \
		&& (ft_str_len_until_equal(str) == ft_str_len_until_equal(tmp->value))))/*         ---> condition non intelligible --> fonction         */
		{
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
