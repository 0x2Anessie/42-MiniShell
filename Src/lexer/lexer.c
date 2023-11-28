
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
		else if (tmp->prev->token == Dflecheg)
			tmp->token = LIMITOR;
		else if ((tmp->prev->token == FD || tmp->prev->token == LIMITOR) // (echo ok <>| gerp ok)
			&& (!tmp->next || tmp->next->word[0] == '|' || tmp->next))
			truc(tmp, first);
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

	state = ft_malloc(sizeof(t_quote));
	if (!state)
		return (0);
	word = NULL;
	x = i;
	k = 0;
	j = 0;
	state_at_zero(state);
	ft_lexer4(str, &i, &j, state); // len du prochain mon en verif les ' et "
	word = ft_malloc(sizeof(char) * (j + 1));
	if (!word)
		return (0);
	word[j] = '\0';
	while (k < j)
		word[k++] = str[x++];
	ft_add_back_lexer(lexer_list, word); // aloue le memoire et ajoute le mot a le fin de la liste
	get_data_in_node(lexer_list); // verif pipe 
	get_token_in_node(lexer_list, tmp); //separe ARG CMD TOKEN 
	return (j);
}

//boucle pour separer toute la ligne et une creer une structure pour chaque mots avec "le mot, ca valeur(ARG,TOKEN,CMD), i"
void	ft_lexer2(t_data *data, t_lexer *tmp, t_lexer *current, int i)
{
	int	j;
	int	x;

	x = 0;
	while (data->line[i])
	{
		j = 0;
		while (data->line[i] == ' ' || \
		(data->line[i] >= 9 && data->line[i] <= 14))
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

//init et ft_lexer2 fait tout
void	ft_lexer(t_data *data)
{
	int		i;
	t_lexer	*current;
	t_lexer	*tmp;

	tmp = NULL;
	current = NULL;
	data->lexer_list = NULL;
	data->nb_mots = ft_nombre_mots(data->line);
	i = 0;
	ft_lexer2(data, tmp, current, i);
}
