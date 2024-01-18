#include "../../include/minishell.h"

// change le repertoire courant vers le dir
int	change_directory(char *dir)
{
	if (chdir(dir) == -1)
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_globi = 1;
		return (0);
	}
	return (1);
}

// change le repertoire courant vers celui indiquer par OLDPWD
int	change_directory_for_oldpwd(t_env *tmp, t_data *data)
{
	if (chdir(get_old_pwd(tmp, data) + 7) == -1)
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_globi = 1;
		return (0);
	}
	return (1);
}

// recupere et retourn la variable d'env PWD, renvoi NULL si il le trouve pas
char	*get_pwd_env(t_env	*tmp, t_data *data)
{
	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (!strncmp(ENV_SET_CURRENT_WORKING_DIR,
				tmp->var_env_name_and_value, 4))
			return (tmp->var_env_name_and_value);
		tmp = tmp->next_var_env_name_and_value;
	}
	return (NULL);
}

/*
	verifie et met a jour la variable d'env OLDPWD si elle est trouvee,
	 renvoie 1 si la
	maj a etait faite et 0 sinon
*/
int	verif_oldpwd(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (ft_strncmp(tmp->var_env_name_and_value, ENV_SET_OLDPWD,
				ft_str_len_until_equal(ENV_SET_OLDPWD)) == 0)
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_SET_OLDPWD, str, data);
			tmp->var_env_name_and_value = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next_var_env_name_and_value;
	}
	return (0);
}
