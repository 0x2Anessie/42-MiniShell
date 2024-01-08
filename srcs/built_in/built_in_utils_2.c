
#include "../../include/minishell.h"

int	ft_string_length(char *s)
{
	int	indx;

	indx = ZERO_INIT;
	if (!s)
		return (0);
	while (s[indx])
		indx++;
	return (indx);
}

char	*ft_strjoin_with_memory_tracking(char *s1, char *s2, t_data *data)
{
	char	*s3;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (!s1 || !s2 || !s1[0] || !s2[0])
		return (NULL);
	s3 = ft_malloc_with_tracking(\
	data, sizeof(char) * (ft_string_length(s1) + ft_string_length(s2) + 1));
	if (!s3)
		return (NULL);
	while (s1 && s1[++j])
		s3[++i] = s1[j];
	j = -1;
	while (s2 && s2[++j])
		s3[++i] = s2[j];
	s3[++i] = '\0';
	return (s3);
}

size_t	ft_str_len_until_equal(char *str)
{
	size_t	indx;

	indx = ZERO_INIT;
	while (str[indx])
	{
		if (str[indx] == '=')
			return (indx);
		indx++;
	}
	return (indx);
}

char	*ft_strjoin_free_arg2_with_memory_tracking(\
char *s1, char *s2, t_data *data)
{
	int		i;
	int		j;
	char	*str;

	i = -1;
	j = ZERO_INIT;
	if (!s1 && s2 && *s2)
		return (ft_strdup(data, s2));
	if (!s2)
		return (NULL);
	str = (char *)ft_malloc_with_tracking(data, sizeof(char)
			*(ft_string_length(s1) + ft_string_length(s2) + 1));
	if (str == NULL)
		return (NULL);
	while (s1[++i] != '\0')
		str[i] = s1[i];
	while (s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	free(s2);
	return (str);
}
