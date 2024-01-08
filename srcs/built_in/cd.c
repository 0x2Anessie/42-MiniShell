
#include "../../include/minishell.h"

/*
	verifi pwd dans la liste des variable d'env, et la met a jour
*/
int	verif_pwd(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	if (str == NULL)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(\
		tmp->var_env_name_and_value, ENV_SET_CURRENT_WORKING_DIR, \
		ft_str_len_until_equal(ENV_SET_CURRENT_WORKING_DIR)) == 0)/*         ---> condition non intelligible --> fonction         */
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_SET_CURRENT_WORKING_DIR, str, data);
			tmp->var_env_name_and_value = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next_var_env_name_and_value;
	}
	return (0);
}

/*
	cherche HOME dans les variable d'env et la retourne si elle est trouver ou NULL sinon
*/
char	*get_home(t_env	*tmp, t_data *data)
{
	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (!strncmp(ENV_SET_USER_HOME_DIR, tmp->var_env_name_and_value, 5))/*         ---> condition non intelligible --> fonction         */
			return (tmp->var_env_name_and_value);
		tmp = tmp->next_var_env_name_and_value;
	}
	return (NULL);
}

/*
	pareil pour OLDPWD
*/
char	*get_old_pwd(t_env	*tmp, t_data *data)
{
	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (!strncmp(ENV_SET_OLDPWD, tmp->var_env_name_and_value, 6))/*         ---> condition non intelligible --> fonction         */
			return (tmp->var_env_name_and_value);
		tmp = tmp->next_var_env_name_and_value;
	}
	return (NULL);
}

/*
	verifi HOME est la met a jour
*/
int	verif_home(char *str, t_data *data)
{
	t_env	*tmp;
	char	*s1;

	tmp = data->utils->linked_list_full_env_var_copy_alpha;
	while (tmp)
	{
		if (ft_strncmp(\
		tmp->var_env_name_and_value, ENV_SET_USER_HOME_DIR, \
		ft_str_len_until_equal(ENV_SET_USER_HOME_DIR)) == 0)/*         ---> condition non intelligible --> fonction         */
		{
			s1 = ft_strjoin_free_arg2_with_memory_tracking(\
			ENV_SET_USER_HOME_DIR, str, data);
			tmp->var_env_name_and_value = create_new_var(s1, data);
			return (1);
		}
		tmp = tmp->next_var_env_name_and_value;
	}
	return (0);
}

/*
	change le repertoire courant par homme et gere les erreur chdir
*/
int	change_directory_for_home(t_env *tmp, t_data *data)
{
	if (chdir(get_home(tmp, data) + 5) == -1)/*         ---> condition non intelligible --> fonction         */
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_signal_received = 1;
		return (0);
	}
	return (1);
}
