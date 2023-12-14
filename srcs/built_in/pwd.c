
#include "../../include/minishell.h"

/*
	elle cherche si il y un pwd, si il y a que pwd elle appel display_pwd
	sinon elle appel display_pwd_error
*/

int	get_pwd(char *tab, t_data *data)
{
	char	**str;

	str = ft_split_mini(tab, ' ', data);
	if (tab && strcmp(str[0], "pwd") == 0)
	{
		if (str[1] == NULL)
			display_pwd();
		else
			display_pwd_error();
	}
	return (0);
}

/*
	elle va afficher la ou on est (repertoir de travail actuel)
	use getcwd pour ca et gere les erreur si il echoue
*/

void	display_pwd(void)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
	{
		ft_printf("error retrieving current directory: " \
		"No such file or directory\n");
		g_all.utils->err = 1;
		free(tmp);
		return ;
	}
	if (g_all.utils->node->out > 0)
	{
		ft_write_fd(tmp, g_all.utils->node->out);
		ft_write_fd("\n", g_all.utils->node->out);
	}
	else if (!g_all.utils->node->out_fail)
	{
		printf("%s\n", tmp);
	}
	g_all.utils->err = 0;
	free(tmp);
}

/*
	affiche une erreur si pwd est use avec des arugument
*/

void	display_pwd_error(void)
{
	if (g_all.utils->node->out > 0)
	{
		ft_write_fd("pwd: too many arguments", g_all.utils->node->out);
	}
	else if (!g_all.utils->node->out_fail)
	{
		printf("pwd: too many arguments");
	}
	g_all.utils->err = 1;
}

/*
	OLDPWD stock le chemin du dernier repertoire de travail
	parcour la list des variable d'env et change de repertoire si il trouve OLDPWD
	OLDPWD se souvient de ou tu es avant de faire pwd, pour y revenir si on fait un cd
*/

void	find_old_pwd(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD", 6))
		{
			change_directory_for_oldpwd(env);
			break ;
		}
		tmp = tmp->next;
	}
}
