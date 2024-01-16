#include "../../include/minishell.h"

int	stop_pand_slash(char *str, char c)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
	{
		if ((str[0] == c && str[1] == '\0'))
		{
			printf("salut22222\n");
			return (0);
		}
		if ((str[0] == c && str[1] == c))
		{
			printf("salut22222\n");
			return (0);
		}
		if ((str[0] == c && str[1] == '.'))
		{
			printf("salut22222\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_tiret(char *str)
{
	if (!stop_pand_slash(str, '/'))
	{
		printf ("bash: %s : is an empty directory\n", str);
		return (0);
	}
	return (1);
}

int	chevron_pipe(char *str)
{
	int	i;

	i = ZERO_INIT;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (str[i - 1] == '>' && str[i + 1] == '<')
			{
				printf("TTTTTT zsh: parse error near `|'\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}
