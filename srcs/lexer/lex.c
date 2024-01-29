/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:23:15 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 11:39:10 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Objectif: Écrire une chaîne de caractères dans un file descriptor spécifié.
	Elle utilise write pour écrire la chaîne dans le file descriptor.
	En cas d'erreur, affiche le message d'erreur "no space left on device"
	et met à jour la variable globale g_globi.
*/
int	ft_write_fd(char *str, int fd)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
		i++;
	if (write (fd, str, i) == FAIL)
	{
		ft_printf(ERR_WRIT_NO_SPAC_LEFT_DEVC);
		g_globi = 1;
		return (1);
	}
	return (0);
}

/*
	Objectif: Ajouter un nouveau token à la fin de la liste des lexers.
	Crée un nouveau lexer avec create_new_lexer.
	Si la liste des lexers est vide, le nouveau lexer devient le premier
	élément.
	Sinon, parcourt la liste pour trouver le dernier élément et y ajoute
	le nouveau lexer.
*/
void	add_lexer_to_end(t_data *data, char *str)
{
	t_lexer	*new;
	t_lexer	*current;

	new = create_new_lexer(data, str);
	if (data->lexer_list == NULL)
	{
		data->lexer_list = new;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		current = data->lexer_list;
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
		new->next = NULL;
	}
	return ;
}

/*
	Objectif: Identifier les limites des mots ou des segments dans la commande
	de l'utilisateur, tout en gérant correctement les cas spéciaux liés aux
	citations.
*/
void	process_lexer_input(char *str, int *i, int *j, t_quote *state)
{
	while (str[*i])
	{
		update_quoting_state(str[*i], state);
		if (!is_white_space(str[*i])
			&& (!state->doubl_quot_status && !state->singl_quot_status))
		{
			(*j)++;
			(*i)++;
		}
		else if (is_white_space(str[*i])
			&& (!state->doubl_quot_status && !state->singl_quot_status))
			break ;
		else if (state->doubl_quot_status || state->singl_quot_status)
		{
			(*j)++;
			(*i)++;
		}
	}
}
