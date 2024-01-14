#include "../includes/ft_printf.h"
#include <unistd.h>

int	ft_putchar(char c)
{
	write(2, &c, 1);
	return (1);
}
