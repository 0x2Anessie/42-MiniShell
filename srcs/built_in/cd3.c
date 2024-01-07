
#include "../../include/minishell.h"

// change le repertoire courant vers le dir
int	change_directory(char *dir)
{
	if (chdir(dir) == -1)/*         ---> condition non intelligible --> fonction         */
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_signal_received = 1;
		return (0);
	}
	return (1);
}

// change le repertoire courant vers celui indiquer par OLDPWD
int	change_directory_for_oldpwd(t_env *tmp, t_data *data)
{
	if (chdir(get_old_pwd(tmp, data) + 7) == -1)/*         ---> condition non intelligible --> fonction         */
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_signal_received = 1;
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
		if (!strncmp(ENV_SET_CURRENT_WORKING_DIR, tmp->content, 4))/*         ---> condition non intelligible --> fonction         */
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

	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, ENV_PREVIOUS_WORKING_DIR,
				ft_str_len_until_equal(ENV_PREVIOUS_WORKING_DIR)) == 0)/*         ---> condition non intelligible --> fonction         */
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_PREVIOUS_WORKING_DIR, str, data);
			tmp->content = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
