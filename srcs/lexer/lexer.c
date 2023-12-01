/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgoure <lgoure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:59:15 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/12/01 14:51:53 by lgoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	get_data_in_node(t_lexer **lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 1;
	tmp = *lexer_list;
	while (tmp)
	{
		if (tmp->word[0] == '|')
			i = 0;
		tmp->i = i;
		tmp = tmp->next;
		i++;
	}
}

void	get_token_in_node(t_lexer **lexer_list, t_lexer *tmp)
{
	t_lexer	*first;

	tmp = *lexer_list;
	while (tmp)
	{
		if (tmp->i == 1)
			first = tmp;
		if (tmp->i == 1 && tmp->word[0] != '<' && tmp->word[0] != '>') // a gere inon token ";" "()" "&" sont considere comme des commandes 
			tmp->token = CMD;
		else if (tmp->i == 0)
			tmp->token = PIPE;
		else if (tmp->word[0] == '<' || tmp->word[0] == '>')
			tmp->token = which_redir(tmp);
		// else if (tmp->prev->token == Dfleched || tmp->prev->token == flecheg
		// 	|| tmp->prev->token == fleched)
		else if (tmp->prev->token == Dfleched || tmp->prev->token == flecheg
			|| tmp->prev->token == fleched || tmp->prev->token == flechegd)
			tmp->token = FD;
		else if (tmp->prev->token == HERE_DOC)
			tmp->token = DELIMITER;
		else if ((tmp->prev->token == FD || tmp->prev->token == DELIMITER) // (echo ok <>| gerp ok)
			&& (!tmp->next || tmp->next->word[0] == '|' || tmp->next))
			assign_command_or_argument_token(tmp, first);
		else
			tmp->token = ARG;
		tmp = tmp->next;
	}
}

int	get_word_in_list(char *str, int i, t_lexer **lexer_list, t_lexer *tmp)
{
	char	*word;
	int		j;
	int		k;
	int		x;
	t_quote	*state;

	state = ft_malloc_with_tracking(sizeof(t_quote));
	if (!state)
		return (0);
	word = NULL;
	x = i;
	k = 0;
	j = 0;
	reset_quoting_state(state);
	process_lexer_input(str, &i, &j, state); // len du prochain mon en verif les ' et "
	word = ft_malloc_with_tracking(sizeof(char) * (j + sizeof('\0')));
	if (!word)
		return (0);
	word[j] = '\0';
	while (k < j)
		word[k++] = str[x++];
	add_lexer_to_end(lexer_list, word); // aloue le memoire et ajoute le mot a le fin de la liste
	get_data_in_node(lexer_list); // verif pipe 
	get_token_in_node(lexer_list, tmp); //separe ARG CMD TOKEN 
	return (j);
}

//boucle pour separer toute la ligne et une creer une structure pour chaque mots avec "le mot, ca valeur(ARG,TOKEN,CMD), i"
void	process_input_string(t_data *data, t_lexer *tmp, t_lexer *current, int i)
{
	int	j;
	int	x;

	x = 0;
	while (data->line[i])
	{
		j = 0;
		while (data->line[i] == ' ' || \
		(data->line[i] >= TAB && data->line[i] <= SHIFT_OUT))
			i++;
		if (!data->line || !data->line[i]) // plus de mot donc faut sortir et verif les token pour erreur
		{
			data->lexer_list = current;
			get_token_in_node(&current, tmp);
			return ;
		}
		j = get_word_in_list(data->line, i, &data->lexer_list,
				tmp); // renvoit la fin du mots
		if (x == 0)
			current = data->lexer_list;
		i = i + j;
		x++;
	}
	data->lexer_list = current;
}

//init et process_input_string fait tout
void	ft_init_lexer_process(t_data *data)
{
	int		i;
	t_lexer	*current;
	t_lexer	*tmp;

	tmp = NULL;
	current = NULL;
	data->lexer_list = NULL;
	data->nb_mots = count_words_in_input(data->line);
	i = ZERO_INIT;
	process_input_string(data, tmp, current, i);
}
