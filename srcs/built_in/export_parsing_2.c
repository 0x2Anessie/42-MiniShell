
#include "../../include/minishell.h"

int	is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_parsing_char(char *str, int i)
{
	if (str[i] == '[' || str[i] == ']' || str[i] == '-' || str[i] == '.'
		|| str[i] == '{' || str[i] == '}' || str[i] == '*' || str[i] == '#'
		|| str[i] == '@' || str[i] == '!' || str[i] == '`' || str[i] == ';'
		|| str[i] == '\\' || str[i] == '&' || str[i] == '^' || str[i] == '%'
		|| str[i] == '$' || str[i] == '(' || str[i] == ')' || str[i] == '='
		|| str[i] == '+' || str[i] == '<' || str[i] == '>' || str[i] == '?'
		|| str[i] == '/' || str[i] == ':' || str[i] == ',' || str[i] == '\''
		|| str[i] == '\"')
	{
		ft_printf("export : `%s': not a valid identifier\n", str);
		return (1);
	}
	return (0);
}

int	check_parsing_2(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '!' || str[i] == '$')
		{
			str += i;
			globi = 1;
			ft_printf("export: %s event not found", str);
			return (1);
		}
		if (check_parsing_char(str, i) == 1)
			return (1);
		if (((str[0] == '"' && str[1] == '"') || (str[0] == '\''
					&& str[1] == '\'')) && str[2] == '\0')
		{
			globi = 1;
			ft_printf("export : `': not a valid identifier\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_parsing_export(char *str)
{
	if (*str == '=' || *str == '/' || *str == ' ' || *str == '.'
		|| is_number(*str) || *str == '+' || *str == '%' || *str == '?'
		|| (*str == '-' && !str[1]))
	{
		globi = 1;
		ft_printf("export : `%s': not a valid identifier\n", str);
		return (1);
	}
	else if (*str == '-')
	{
		globi = 2;
		ft_printf("export : %c%c: invalid option\n", str[0], str[1]);
		return (1);
	}
	if (check_parsing_2(str) == 1)
		return (1);
	return (0);
}
