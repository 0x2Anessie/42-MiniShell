#include "../includes/lib.h"
#include "../../include/minishell.h"

int	strlen2(char *str)
{
	int	i;

	i = 0;
	if (str[i])
	{
		while (str[i])
			i++;
		return (i);
	}
	return (0);
}

char	*ft_strjoin2_mini(char *s1, char *s2, t_data *data)
{
	char	*s3;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (!s1 && !s2)
		return (NULL);
	s3 = ft_malloc_with_tracking(data, sizeof(char) * (strlen2(s1) + strlen2(s2) + 1));
	if (!s3)
		return (NULL);
	while (s1[++j])
		s3[++i] = s1[j];
	j = -1;
	while (s2[++j])
		s3[++i] = s2[j];
	s3[++i] = '\0';
	return (s3);
}

// int	main(void)
// {
// 	char *str;
// 	char *str1;
// 	char *str2;

// 	str2 = "";
// 	str1 = "bonjour";
// 	str = ft_strjoin2(str1, str2);
// 	printf("str = %s\n", str);
// 	free(str);
// }
