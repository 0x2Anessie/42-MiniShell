
#include "../../include/minishell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	char	*dest_start;

	dest_start = dest;
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
	return (dest_start);
}

char	*add_quotes_string(char *str, t_data *data)
{
	size_t	len;
	char	*quoted_str;

	len = (size_t)ft_strlen(str);
	quoted_str = ft_malloc_with_tracking(data, sizeof(char)
			*(len + 3));
	if (!quoted_str)
		return (NULL);
	quoted_str[0] = '"';
	ft_strcpy(quoted_str + 1, str);
	quoted_str[len + 1] = '"';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

char	*join_them(char *str, int i, int flag, t_data *data)
{
	char	*tmp;

	tmp = NULL;
	if (str[i + 1] == '\0' && flag == 1)/*         ---> condition non intelligible --> fonction         */
	{
		tmp = ft_substr(data, (const char *)str,
				(unsigned int)i + 1, (size_t)ft_strlen(str));
		str = ft_substr(data, str, 0, (size_t)i + 1);
		tmp = add_quotes_string(tmp, data);
		str = ft_strjoin_with_memory_tracking(str, tmp, data);
	}
	else if (flag == 1)
	{
		tmp = ft_substr(data, (const char *)str,
				(unsigned int)i + 1, (size_t)ft_strlen(str));
		str = ft_substr(data, str, 0, (size_t)i + 1);
		tmp = add_quotes_string(tmp, data);
		str = ft_strjoin_with_memory_tracking(str, tmp, data);
	}
	return (str);
}

char	*case_egale(char *str, t_data *data)
{
	int		i;
	int		flag;

	i = ZERO_INIT;
	flag = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			flag++;
			break ;
		}
		i++;
	}
	str = join_them(str, i, flag, data);
	return (str);
}

void	exit_all(t_data *data)
{
	ft_write_fd("exit\n", STDOUT_FILENO);
	ft_free_all(data);
	exit(0);
}
