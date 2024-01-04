
#include "../../include/minishell.h"

// int	verif_pwd(char *str)
// {
// 	t_env	*tmp;
// 	char	*s1;

// 	tmp = data->utils->env_lst;
// 	while (tmp)
// 	{
// 		if (ft_strncmp(tmp->content, ENV_SET_CURRENT_WORKING_DIR, ft_str_len_until_equal(ENV_SET_CURRENT_WORKING_DIR)) == 0)
// 		{
// 			s1 = ft_strjoin_free_arg2_with_memory_tracking(ENV_SET_CURRENT_WORKING_DIR, str);
// 			tmp->content = create_new_var(s1);
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

// int	verif_oldpwd(char *str)
// {
// 	t_env	*tmp;
// 	char	*s1;

// 	tmp = data->utils->env_lst;
// 	while (tmp)
// 	{
// 		if (ft_strncmp(tmp->content, "OLDPWD=",
// 				ft_str_len_until_equal("OLDPWD=")) == 0)
// 		{
// 			s1 = ft_strjoin_free_arg2_with_memory_tracking("OLDPWD=", str);
// 			tmp->content = create_new_var(s1);
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

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

	tmp = data->utils->export_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, ENV_SET_OLDPWD, ft_str_len_until_equal(ENV_SET_OLDPWD)) == 0)
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(ENV_SET_OLDPWD, str, data);
			tmp->value = create_new_var(s1, data);
			tmp->value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

// la meme chose pour pwd
int	verif_pwd_export(char *str, t_data *data)
{
	t_export	*tmp;
	char		*s1;

	tmp = data->utils->export_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, ENV_SET_CURRENT_WORKING_DIR, ft_str_len_until_equal(ENV_SET_CURRENT_WORKING_DIR)) == 0)
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(ENV_SET_CURRENT_WORKING_DIR, str, data);
			tmp->value = create_new_var(s1, data);
			tmp->value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
