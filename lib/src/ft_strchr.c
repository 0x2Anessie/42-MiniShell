/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:09:40 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:09:42 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lib.h"

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
	}
	return (0);
}
