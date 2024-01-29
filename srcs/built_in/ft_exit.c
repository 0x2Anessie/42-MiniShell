/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:34 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:36 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*check si le nombre est superieur a 19 ou au long long puis renvoie
une erreur si cest le cas
*/
int	is_valid_number2(int neg, char *str, int i)
{
	if (str[0] == '+' || str[0] == '-')
		i--;
	if (i > 19)
		return (1);
	if (i == 19)
	{
		if (str[0] == '-' || str[0] == '+')
			str++;
		if (neg && ft_strcmp(str, "9223372036854775808") > 0)
			return (1);
		if (!neg && ft_strcmp(str, "9223372036854775807") > 0)
			return (1);
	}
	return (0);
}

// gere le + et -  lui et celle d'avant serve a verifier le nb du code d'erreur
int	is_valid_number(char *str)
{
	int	i;
	int	neg;

	neg = 0;
	i = -1;
	while (str[++i])
	{
		if (!i && str[i] == '-')
		{
			neg = 1;
			continue ;
		}
		if (!i && str[i] == '+')
			continue ;
		if ((str[i] < '0' || str[i] > '9'))
			return (1);
	}
	return (is_valid_number2(neg, str, i));
}

// ferme les pipes et descripteurs de fichier ouvert avant l'exit
void	free_res(int *fd, t_data *data)
{
	closes_ends_of_pipe(fd);
	close_file_descriptors_in_linked_list(data);
	ft_free_all(data);
}

// check i il y a trop d'argument et sinons'occupe de sortir
void	ft_exit_with_code(t_lexer *lex, int *fd, unsigned char exit_code,
	t_data *data)
{
	if (lex->next->next)
	{
		ft_printf("minishell: exit: too many arguments\n");
		g_globi = 1;
	}
	else
	{
		exit_code = ft_atoll(lex->next->cmd_segment);
		free_res(fd, data);
		exit (exit_code);
	}
}

/*
	fait l'exit, affiche exit si c le seul shell en cour d'execution puis
	 verifie si le code
	de sorti est correct puis envoie a exit_with_code pour quitter avec le
	bon code de sorti ou 0 si
	aucun code specifier
*/
void	ft_exit(t_lexer *lex, int *fd, t_data *data)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (data->utils->nb_cmd_in_lst == 1)
		write(1, "exit\n", 5);
	if (lex->next)
	{
		if (is_valid_number(lex->next->cmd_segment))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", \
			lex->next->cmd_segment);
			free_res(fd, data);
			exit(2);
		}
		else
			ft_exit_with_code(lex, fd, exit_code, data);
	}
	else
	{
		free_res(fd, data);
		exit (exit_code);
	}
}
