#include "../includes/lib.h"

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
