
#include "../../include/minishell.h"

// int	verif_pwd(char *str)
// {
// 	t_env	*tmp;
// 	char	*s1;

// 	tmp = data->utils->env_lst;
// 	while (tmp)
// 	{
// 		if (ft_strncmp(tmp->content, "PWD=", ft_strlen_eguale("PWD=")) == 0)
// 		{
// 			s1 = ft_strjoin_2("PWD=", str);
// 			tmp->content = var_exist(s1);
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
// 				ft_strlen_eguale("OLDPWD=")) == 0)
// 		{
// 			s1 = ft_strjoin_2("OLDPWD=", str);
// 			tmp->content = var_exist(s1);
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

int	verif_oldpwd_export(char *str, t_data *data)
{
	t_export	*tmp;
	char		*s1;

	tmp = data->utils->export_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, "OLDPWD=", ft_strlen_eguale("OLDPWD=")) == 0)
		{
			s1 = ft_strjoin_2("OLDPWD=", str, data);
			tmp->value = var_exist(s1, data);
			tmp->value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	verif_pwd_export(char *str, t_data *data)
{
	t_export	*tmp;
	char		*s1;

	tmp = data->utils->export_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, "PWD=", ft_strlen_eguale("PWD=")) == 0)
		{
			s1 = ft_strjoin_2("PWD=", str, data);
			tmp->value = var_exist(s1, data);
			tmp->value = case_egale(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
