
#include "../../include/minishell.h"

int	is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

// liste des caractere interdit pour un identifiant de variable
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

/*
	parcour la chaine jusqu'a un = et use checl_parsing_char pour verif chaque char
	et gere les erreurs comme chaine vide ou contenant juste des guillemet
*/
int	check_parsing_error(char *str)
{
	int	i;

	i = ZERO_INIT;
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

/*
	Vérifie la validité globale d'un argument passé à la commande export
	verifie les conditions sur les premier caractere de str au cas ou il y a
	une erreur puis appel check_parsing_error pour finir
*/
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
	if (check_parsing_error(str) == 1)
		return (1);
	return (0);
}
