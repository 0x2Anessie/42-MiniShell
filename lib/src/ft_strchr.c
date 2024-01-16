#include "../includes/lib.h"

/*
int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}
*/

char *ft_strchr(char *str, int c)
{
	char ch = c;
	int index = 0;

	while (str[index] != '\0')
	{
		if (str[index] == ch)
			return (char *)(str + index);
		index++;
	}
	return 0;
}
