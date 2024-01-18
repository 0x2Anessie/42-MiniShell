#include "../../include/minishell.h"

void	freetab(char **tab, int k)
{
	while (k >= 0)
	{
		free(tab[k]);
		k--;
	}
	free(tab);
	return ;
}

int	ft_word_count(char *s, char c)
{
	int	i;
	int	count;

	i = ZERO_INIT;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

char	*ft_put_word_in_malloc(char *s, char c, char *tab)
{
	int	i;

	i = ZERO_INIT;
	while (s[i] && (s[i] != c))
	{
		tab[i] = s[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

char	**ft_malloc_word(char **tab, char *s, char c, t_data *data)
{
	int	i;
	int	j;
	int	k;

	k = ZERO_INIT;
	i = ZERO_INIT;
	while (s[i])
	{
		if (s[i] == c || s[i] == '\0')
			i++;
		else
		{
			j = 0;
			while (s[i + j] != c && s[i + j])
				j++;
			tab[k] = ft_malloc_with_tracking(data, sizeof(char) * (j + 1));
			if (!tab[k])
				freetab(tab, k - 1);
			tab[k] = ft_put_word_in_malloc(&s[i], c, tab[k]);
			i = i + j;
			k++;
		}
	}
	return (tab);
}

char	**ft_split_mini(char const *s, char c, t_data *data)
{
	char	**tab;
	int		word_count;

	if (!s)
		return (0);
	word_count = ft_word_count((char *)s, c);
	tab = ft_malloc_with_tracking(data, sizeof(char *) * (word_count + 1));
	if (!tab)
		return (NULL);
	tab[word_count] = NULL;
	ft_malloc_word(tab, (char *)s, c, data);
	return (tab);
}
