
#include "../../include/minishell.h"

int	is_valid_number2(int neg, char *str, int i)
{
	if (str[0] == '+' || str[0] == '-')
		i--;
	if (i > 19)
		return (1);
	if (i == 19)
	{
		if (str[0] == '-' || str[0] == '+')
			str++;
		if (neg && ft_strcmp(str, "9223372036854775808") > 0)
			return (1);
		if (!neg && ft_strcmp(str, "9223372036854775807") > 0)
			return (1);
	}
	return (0);
}

int	is_valid_number(char *str)
{
	int	i;
	int	neg;

	neg = 0;
	i = -1;
	while (str[++i])
	{
		if (!i && str[i] == '-')
		{
			neg = 1;
			continue ;
		}
		if (!i && str[i] == '+')
			continue ;
		if ((str[i] < '0' || str[i] > '9'))
			return (1);
	}
	return (is_valid_number2(neg, str, i));
}

void	free_res(int *fd, t_data data)
{
	close_pipe(fd);
	close_fd();
	ft_free_all(data);
}

void	ft_exit2(t_lexer *lex, int *fd, unsigned char exit_code, t_data data)
{
	if (lex->next->next)
	{
		ft_printf("bash: exit: too many arguments\n");
		g_all.utils->err = 1;
	}
	else
	{
		exit_code = ft_atoll(lex->next->word);
		free_res(fd, data);
		exit (exit_code);
	}
}

void	ft_exit(t_lexer *lex, int *fd, t_data data)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (g_all.utils->nb_cmd == 1)
		write(1, "exit\n", 5);
	if (lex->next)
	{
		if (is_valid_number(lex->next->word))
		{
			ft_printf("bash: exit: %s: numeric argument required\n", \
			lex->next->word);
			free_res(fd, data);
			exit(2);
		}
		else
			ft_exit2(lex, fd, exit_code, data);
	}
	else
	{
		free_res(fd, data);
		exit (exit_code);
	}
}
