
#include "../../include/minishell.h"

// change le repertoire courant vers le dir

int	change_directory(char *dir)
{
	if (chdir(dir) == -1)
	{
		perror("chdir");
		g_all.utils->err = 1;
		return (0);
	}
	return (1);
}

// change le repertoire courant vers celui indiquer par OLDPWD

int	change_directory_for_oldpwd(t_env *tmp)
{
	if (chdir(get_old_pwd(tmp) + 7) == -1)
	{
		perror("chdir");
		g_all.utils->err = 1;
		return (0);
	}
	return (1);
}

// recupere et retourn la variable d'env PWD, renvoi NULL si il le trouve pas

char	*get_pwd_env(t_env	*tmp)
{
	tmp = g_all.utils->env_lst;
	while (tmp)
	{
		if (!strncmp("PWD=", tmp->content, 4))
			return (tmp->content);
		tmp = tmp->next;
	}
	return (NULL);
}

/*
	verifie et met a jour la variable d'env OLDPWD si elle est trouvee, renvoie 1 si la
	maj a etait faite et 0 sinon
*/

int	verif_oldpwd(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = g_all.utils->env_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "OLDPWD=",
				ft_strlen_eguale("OLDPWD=")) == 0)
		{
			s1 = ft_strjoin_2("OLDPWD=", str, data);
			tmp->content = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
