#include "../../include/minishell.h"

/*
change le repertoire courant par celui indiquer par pwd toujours avec chdir
et ca gestion d'err
*/
int	change_directory_for_pwd(t_env *tmp, t_data *data)
{
	printf("get_pwd_env(tmp) + 4) %s\n", get_pwd_env(tmp, data) + 4);
	if (chdir(get_pwd_env(tmp, data) + 4) == -1)
	{
		perror(CMD_CHANGE_DIRECTORY);
		g_globi = 1;
		return (0);
	}
	return (1);
}

/*
	check si la chaine contient un = et si oui reconstruit la variable avec la
	nouvelle variable et renvoie la chaine mise a jour
*/
char	*create_new_var(char *str, t_data *data)
{
	int		i;
	int		flag;
	char	*tmp;

	flag = 0;
	tmp = NULL;
	i = ZERO_INIT;
	while (str[i])
	{
		if (str[i] == '=')
		{
			flag++;
			break ;
		}
		i++;
	}
	if (flag)
	{
		tmp = ft_substr(\
		data, str, (unsigned int)i + 1, (size_t)ft_strlen(str));
		str = ft_substr(data, str, 0, (size_t)i + 1);
		str = ft_strjoin_with_memory_tracking(str, tmp, data);
	}
	return (str);
}

/*
	verifie si cd n'a pas d'argument, et dans ce cas la change vers le HOME
	si il y a des argument on appel cd_with_arg
*/
void	get_cd(t_lexer *lexer_lst, t_data *data)
{
	t_env	*env;
	int		i;
	char	*path;
	char	*old;

	i = ZERO_INIT;
	path = NULL;
	old = NULL;
	env = data->utils->linked_list_full_env_var_copy_alpha;
	if (lexer_lst)
	{
		if ((ft_strcmp(lexer_lst->cmd_segment, CMD_CHANG_DIRCT) == 0)
			&& lexer_lst->next == NULL)
		{
			if (change_directory_for_home(env, data))
			{
				path = getcwd(path, i);
				verif_home(path, data);
			}
			return ;
		}
		if (cd_with_arg(data, path, old, &i) == 0)
			return ;
	}
	lexer_lst = data->utils->head_lexer_lst;
}

// check si il y a trop d'argument et gere l'erreur si c'est le cas
int	wrong_cd(t_lexer *lexer_lst)
{
	if (lexer_lst->next)
	{
		write (STDERR_FILENO, "minishell: cd: trop d'arguments\n", 27);
		g_globi = 1;
		return (0);
	}
	return (1);
}

/*
	traite cd avec des arguments, recupere le repertoire courant et le stock
	dans le OLDPWD et si les arguments sont valid, change le repertoire et met
	a jour PWD
*/
int	cd_with_arg(t_data *data, char *path, char *old, int *i)
{
	if (((ft_strcmp(data->lexer_list->cmd_segment, CMD_CHANG_DIRCT) == 0)
			&& data->lexer_list->next->cmd_segment))
	{
		if (data->lexer_list->next)
			data->lexer_list = data->lexer_list->next;
		old = getcwd(old, *i);
		if (!old)
		{
			find_old_pwd(\
			data->utils->linked_list_full_env_var_copy_alpha, data);
			return (0);
		}
		else
			verif_oldpwd(old, data);
		if (!wrong_cd(data->lexer_list))
			return (0);
		if (change_directory(data->lexer_list->cmd_segment))
		{
			path = getcwd(path, *i);
			verif_pwd(path, data);
			g_globi = 0;
		}
	}
	return (1);
}
