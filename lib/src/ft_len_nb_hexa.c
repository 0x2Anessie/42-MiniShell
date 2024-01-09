#include "../includes/ft_printf.h"

int	ft_len_nb_hexa(unsigned long n)
{
	unsigned long	i;

	i = 0;
	if (n == 0)
	{
		i = 1;
		return (i);
	}
	while (n != 0)
	{
		i++;
		n = n / 16;
	}
	return (i);
}
