
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

int	get_word_in_list(char *str, int i, t_data *data, t_lexer *tmp)
{
	char	*word;
	int		j;
	int		k;
	int		x;
	t_quote	*state;

	state = ft_malloc_with_tracking(data, sizeof(t_quote));
	if (!state)
		return (0);
	word = NULL;
	x = i;
	k = 0;
	j = 0;
	reset_quoting_state(state); // initialise les quote
	process_lexer_input(str, &i, &j, state); // len du prochain mon en verif les ' et "
	word = ft_malloc_with_tracking(data, sizeof(char) * (j + sizeof('\0')));
	if (!word)
		return (0);
	word[j] = '\0';
	while (k < j)
		word[k++] = str[x++];
	add_lexer_to_end(data, word); // aloue le memoire et ajoute le mot a le fin de la liste
	get_data_in_node(&data->lexer_list); // verif pipe 
	get_token_in_node(&data->lexer_list, tmp); //separe ARG CMD TOKEN 
	if (data->lexer_list)
		printf("niveau 2\n");
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
			if (data->lexer_list)
				printf("niveau 1\n");
			return ;
		}
		if (x == 0)
			current = data->lexer_list;
		j = get_word_in_list(data->line, i, data,
				tmp); // renvoit la fin du mots et separe tout
		i = i + j;
		x++;
	}
	data->lexer_list = current;
	if (data->lexer_list)
		printf("niveau 1\n");
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
	if (data->lexer_list)
		printf("niveau 0\n");
}
