#include "../includes/ft_printf.h"

int	ft_len_nb(long n)
{
	int	i;

	i = 0;
	if (n == 0)
	{
		i = 1;
		return (i);
	}
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}
