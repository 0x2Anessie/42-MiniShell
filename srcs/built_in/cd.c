
#include "../../include/minishell.h"

int	verif_pwd(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = data->utils->env_lst;
	if (str == NULL)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "PWD=", ft_strlen_eguale("PWD=")) == 0)
		{
			s1 = ft_strjoin_2("PWD=", str, data);
			tmp->content = var_exist(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	*get_home(t_env	*tmp, t_data *data)
{
	tmp = data->utils->env_lst;
	while (tmp)
	{
		if (!strncmp("HOME=", tmp->content, 5))
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_old_pwd(t_env	*tmp, t_data *data)
{
	tmp = data->utils->env_lst;
	while (tmp)
	{
		if (!strncmp("OLDPWD=", tmp->content, 6))
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

int	verif_home(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = data->utils->env_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "HOME=", ft_strlen_eguale("HOME=")) == 0)
		{
			s1 = ft_strjoin_2("HOME=", str, data);
			tmp->content = var_exist(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	change_directory2(t_env *tmp, t_data *data)
{
	if (chdir(get_home(tmp, data) + 5) == -1)
	{
		perror("chdir");
		globi = 1;
		return (0);
	}
	return (1);
}
