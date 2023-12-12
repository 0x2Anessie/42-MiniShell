
#include "../../include/minishell.h"

int	change_directory4(t_env *tmp)
{
	printf("get_pwd_env(tmp) + 4) %s\n", get_pwd_env(tmp) + 4);
	if (chdir(get_pwd_env(tmp) + 4) == -1)
	{
		perror("chdir");
		g_all.utils->err = 1;
		return (0);
	}
	return (1);
}

char	*var_exist(char *str, t_data *data)
{
	int		i;
	int		flag;
	char	*tmp;

	flag = 0;
	tmp = NULL;
	i = 0;
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
		tmp = ft_substr(data, str, (unsigned int)i + 1, (size_t)ft_strlen(str));
		str = ft_substr(data, str, 0, (size_t)i + 1);
		str = ft_strjoin4(str, tmp, data);
	}
	return (str);
}

void	get_cd(t_lexer *lexer_lst, t_data *data)
{
	t_env	*env;
	int		i;
	char	*path;
	char	*old;

	i = 0;
	path = NULL;
	old = NULL;
	env = g_all.utils->env_lst;
	if (lexer_lst)
	{
		if ((ft_strcmp(lexer_lst->word, "cd") == 0)
			&& lexer_lst->next == NULL)
		{
			if (change_directory2(env))
			{
				path = getcwd(path, i);
				verif_home(path, data);
			}
			return ;
		}
		if (cd_2(data, path, old, &i) == 0)
			return ;
	}
	lexer_lst = g_all.utils->head_lexer_lst;
}

int	wrong_cd(t_lexer *lexer_lst)
{
	if (lexer_lst->next)
	{
		write (2, "bash: cd: trop d'arguments\n", 27);
		g_all.utils->err = 1;
		return (0);
	}
	return (1);
}

int	cd_2(t_data *data, char *path, char *old, int *i)
{
	if (((ft_strcmp(data->lexer_list->word, "cd") == 0)
			&& data->lexer_list->next->word))
	{			
		if (data->lexer_list->next)
				data->lexer_list = data->lexer_list->next;
		old = getcwd(old, *i);
		if (!old)
		{
			find_old_pwd(g_all.utils->env_lst);
			return (0);
		}
		else
			verif_oldpwd(old, data);
		if (!wrong_cd(data->lexer_list))
			return (0);
		if (change_directory(data->lexer_list->word))
		{
			path = getcwd(path, *i);
			verif_pwd(path, data);
			g_all.utils->err = 0;
		}
	}
	return (1);
}
