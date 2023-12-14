
#include "../../include/minishell.h"

// verifi pwd dans la liste des variable d'env, et la met a jour

int	verif_pwd(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = g_all.utils->env_lst;
	if (str == NULL)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "PWD=", ft_strlen_eguale("PWD=")) == 0)
		{
			s1 = ft_strjoin_2("PWD=", str, data);
			tmp->content = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

//cherche HOME dans les variable d'env et la retourne si elle est trouver ou NULL sinon

char	*get_home(t_env	*tmp)
{
	tmp = g_all.utils->env_lst;
	while (tmp)
	{
		if (!strncmp("HOME=", tmp->content, 5))
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

// pareil pour OLDPWD

char	*get_old_pwd(t_env	*tmp)
{
	tmp = g_all.utils->env_lst;
	while (tmp)
	{
		if (!strncmp("OLDPWD=", tmp->content, 6))
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

// verifi HOME est la met a jour

int	verif_home(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = g_all.utils->env_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "HOME=", ft_strlen_eguale("HOME=")) == 0)
		{
			s1 = ft_strjoin_2("HOME=", str, data);
			tmp->content = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

// change le repertoire courant par homme et gere les erreur chdir

int	change_directory_for_home(t_env *tmp)
{
	if (chdir(get_home(tmp) + 5) == -1)
	{
		perror("chdir");
		g_all.utils->err = 1;
		return (0);
	}
	return (1);
}
