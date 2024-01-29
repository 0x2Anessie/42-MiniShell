/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:50 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:52 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	ecri dans la sortie les argument apres verif qu'il soit pas un -n puis
	ecrit un espace entre les argumen ,elle est utiliser quand echo doit etre
	afficher ailleurs que dans la sorti standard
*/
void	write_echo_fd(char **tab, int i, t_data *data)
{
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "-n"))
		{
			ft_write_fd(tab[i], data->utils->node->output_fd);
			if (tab[i + 1])
				ft_write_fd(" ", data->utils->node->output_fd);
		}
		i++;
	}
}

/*
	elle est apeller dans process_echo si il y a besoin d'use une sorti special
	si j n'est pas nul la fonction va ecrire un space pour separer les argument
	et n'ecrit rien si j est nul
	procc_in_echo_fd ecrit les argument de echo si il y a besoin d'une
	redirection
*/
int	procc_in_echo_fd(char **tab, int i, int j, t_data *data)
{
	while (tab[i])
	{
		if (j)
		{
			if (ft_write_fd(" ", data->utils->node->output_fd))
			{
				g_globi = 1;
				return (-1);
			}
			ft_write_fd(tab[i++], data->utils->node->output_fd);
		}
		else
		{
			if (ft_write_fd(tab[i++], data->utils->node->output_fd))
			{
				g_globi = 1;
				return (-1);
			}
			j++;
		}
	}
	return (0);
}
