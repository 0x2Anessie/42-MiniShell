/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:22:23 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 11:48:36 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Met à jour globi si la commande saisie n'est pas un répertoire.
*/
int	err_code_directory(t_data *data)
{
	if (!directory_check(data->minishell_input_cmd))
	{
		g_globi = 126;
		return (0);
	}
	return (1);
}

/*
	vérifie si le premier caractère est un slash ou un tiret suivi par
	certains caractères spécifiques.
*/
int	slash_check(char *str, char c)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
	{
		if ((str[0] == c && str[1] == '\0'))
			return (0);
		if ((str[0] == c && str[1] == c))
			return (0);
		if ((str[0] == c && str[1] == '.'))
			return (0);
		i++;
	}
	return (1);
}

/*
	Détermine si le segment de commande est un répertoire.
*/
int	directory_check(char *str)
{
	if (!slash_check(str, '/'))
	{
		printf ("minishell: %s: Is a directory\n", str);
		return (0);
	}
	return (1);
}

/*
	Vérifie des combinaisons spécifiques de chevrons et de pipes qui sont
	syntaxiquement incorrectes.
*/
int	pipe_bracket_check(char *str)
{
	int	i;

	i = ZERO_INIT;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (str[i - 1] == '>' && str[i + 1] == '<')
			{
				printf("minishell: parse error near `|'\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}
