
#include "../../include/minishell.h"

t_lexer	*create_new_lexer(char *str)
{
	t_lexer	*new;

	new = ft_malloc_with_tracking(sizeof(t_lexer));
	if (!new)
		return (NULL);
	new->word = str;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	check_redir_error(t_lexer *tmp) // je pense que ca sert a rein a tester
{
	int	i;

	i = -1;
	if (tmp->word[0] == '<')
		if (tmp->word[i] != '>')
				return (0);
	if (tmp->word[0] == '>')
		while (tmp->word[++i])
			if (tmp->word[i] != '>')
				return (-1);
	if (i > 2)
		return (-1);
	if (tmp->word[0] == '<')
		while (tmp->word[++i])
			if (tmp->word[i] != '<')
				return (-1);
	if (i > 2)
		return (-1);
	return (0);
}

t_token	which_redir(t_lexer *tmp)
{
	if (!check_redir_error(tmp))
	{
		if (tmp->word[0] == '>' && tmp->word[1] == '\0')
			return (fleched);
		else if (tmp->word[0] == '<' && tmp->word[1] == '\0')
			return (flecheg);
		else if (tmp->word[0] == '>' && tmp->word[1] == '>')
			return (Dfleched);
		else if (tmp->word[0] == '<' && tmp->word[1] == '<')
			return (HERE_DOC);
		else if (tmp->word[0] == '<' && tmp->word[1] == '>')
			return (flechegd);
		else if (tmp->word[0] == '>' && tmp->word[1] == '<')
			return (flechedg);
	}
	return (-1);
}

void	assign_command_or_argument_token(t_lexer *tmp, t_lexer *first) 
{	
	if (first->token != CMD)
		tmp->token = CMD;
	else
		tmp->token = ARG;
	printf("cccccc111cccc\n");
}

int	count_words_in_input(char *str)
{
	int	i;
	int	count;

	count = ZERO_INIT;
	i = ZERO_INIT;
	while (str[i])
	{
		while (is_white_space(str[i]) && str[i])
			i++;
		if (str[i])
			count++;
		while (!is_white_space(str[i]) && str[i])
			i++;
	}
	return (count);
}
