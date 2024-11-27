#include "../../include/minishell.h"

void	reset_quoting_state(t_quote *state)
{
	state->doubl_quot_status = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
}

int	length_of_number_for_string_conversion(long n)
{
	int	length;

	length = ZERO_INIT;
	if (n == 0)
	{
		length = 1;
		return (length);
	}
	while (n != 0)
	{
		length++;
		n /= 10;
	}
	return (length);
}

char	*convert_negative_int_to_string_with_tracking(\
t_data *data, char *res, long n, int length)
{
	n = n * (-1);
	res = \
	malloc_track(\
	data, sizeof(char) * length + sizeof('-') + sizeof('\0'));
	if (!res)
		return (NULL);
	res[0] = '-';
	res[length + FORWARD_OFFSET] = '\0';
	while (length >= 1)
	{
		res[length] = (n % 10) + '0';
		n /= 10;
		length--;
	}
	return (res);
}

char	*convert_int_to_string_with_tracking(t_data *data, int n)
{
	long	nb;
	int		index;
	char	*res;

	nb = (long)n;
	res = ZERO_INIT;
	index = length_of_number_for_string_conversion(nb);
	if (nb < 0)
	{
		res = \
		convert_negative_int_to_string_with_tracking(data, res, nb, index);
		return (res);
	}
	res = malloc_track(data, sizeof(char) * (index + sizeof('\0')));
	res[index] = '\0';
	if (!res)
		return (NULL);
	index--;
	while (index >= 0)
	{
		res[index] = (nb % 10) + '0';
		nb /= 10;
		index--;
	}
	return (res);
}

int	is_decimal_digit(char c)
{
	return (c >= '0' && c <= '9');
}
