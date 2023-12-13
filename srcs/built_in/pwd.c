
#include "../../include/minishell.h"

int	get_pwd(char *tab, t_data *data)
{
	char	**str;

	str = ft_split_mini(tab, ' ', data);
	if (tab && strcmp(str[0], "pwd") == 0)
	{
		if (str[1] == NULL)
			display_pwd(data);
		else
			display_pwd_error(data);
	}
	return (0);
}

void	display_pwd(t_data *data)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
	{
		ft_printf("error retrieving current directory: " \
		"No such file or directory\n");
		globi = 1;
		free(tmp);
		return ;
	}
	if (data->utils->node->out > 0)
	{
		ft_write_fd(tmp, data->utils->node->out);
		ft_write_fd("\n", data->utils->node->out);
	}
	else if (!data->utils->node->out_fail)
	{
		printf("%s\n", tmp);
	}
	globi = 0;
	free(tmp);
}

void	display_pwd_error(t_data *data)
{
	if (data->utils->node->out > 0)
	{
		ft_write_fd("pwd: too many arguments", data->utils->node->out);
	}
	else if (!data->utils->node->out_fail)
	{
		printf("pwd: too many arguments");
	}
	globi = 1;
}

void	find_old_pwd(t_env *env, t_data *data)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD", 6))
		{
			change_directory3(env, data);
			break ;
		}
		tmp = tmp->next;
	}
}
