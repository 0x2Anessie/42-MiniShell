/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:19:47 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:19:48 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	parcour la list du lexer pour compter le nombre d'arg ou cmd puis
	allou un tableau avec le nombre d'elem trouver puis appel get_words
	pour remplir le tableau puis une fois pret appel sim_echo3
*/
void	init_echo(t_lexer *lexer_lst, t_data *data)
{
	int			i;
	char		**tab;
	t_lexer		*head;

	i = ZERO_INIT;
	tab = NULL;
	head = lexer_lst;
	while ((lexer_lst) && (lexer_lst->token == CMD || lexer_lst->token == ARG))
	{
		lexer_lst = lexer_lst->next;
		i++;
	}
	tab = malloc_track(data, sizeof(char *) * (i + 1));
	if (!tab)
		return ;
	lexer_lst = head;
	i = 0;
	get_words(lexer_lst, tab, &i);
	tab[i] = NULL;
	simulate_echo(tab, data);
}

// verifie si il y a un -n meme avec une ecriture chelou genre -nnnnnnn
int	check_n(char *str)
{
	int	i;

	i = ZERO_INIT;
	if (str[i] == '-')
		i++;
	else
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] && str[i] != 'n')
		return (0);
	return (i);
}

/*
	commence par verifier si il y a un -n apres echo
	puis si il un -n appel display_echo_arg et sinon apell process echo
	enfin si nous somme dans aucun des deux cas la fonction ecrit elle
	meme le retour a la ligne sur la sorti use
*/
void	simulate_echo(char **tab, t_data *data)
{
	int	i;
	int	ncount;

	i = ZERO_INIT;
	if (tab && tab[i] && tab[i + 1])
		ncount = check_n(tab[i + 1]);
	if (tab[i] && tab[i + 1] && !ft_strcmp(tab[i], CMD_ECHO)
		&& tab[i + 1][0] != '-' && tab[i + 1][1] != 'n')
		process_echo(tab, ++i, data);
	else if (tab[i] && tab[i + 1] && !ft_strcmp(tab[i], CMD_ECHO)
		&& ncount)
		display_echo_arg(tab, i, data);
	else
	{
		if (data->utils->node->output_fd > 0)
			ft_write_fd("\n", data->utils->node->output_fd);
		else
			printf("\n");
		g_globi = 0;
	}
}
