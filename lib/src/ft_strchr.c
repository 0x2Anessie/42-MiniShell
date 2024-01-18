#include "../includes/lib.h"

<<<<<<< HEAD
int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
=======
char	*ft_strchr(char *str, int c)
{
	char	ch;
	int		index;

	ch = c;
	index = 0;
	while (str[index] != '\0')
	{
		if (str[index] == ch)
			return ((char *)(str + index));
		index++;
>>>>>>> raf
	}
	return (0);
}
