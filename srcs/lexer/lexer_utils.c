#include "../../include/minishell.h"

/*
	Alloue de l'espace mémoire pour un nouveau lexer et nitialise le lexer
	avec la chaîne str.
*/
t_lexer	*create_new_lexer(t_data *data, char *str)
{
	t_lexer	*new;

	new = malloc_track(data, sizeof(t_lexer));
	if (!new)
		return (NULL);
	new->cmd_segment = str;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

/*
	Vérifie la validité des symboles de redirection dans un segment de commande.
*/
int	check_redir_error(t_lexer *tmp)
{
	int	i;

	i = -1;
	if (tmp->cmd_segment[0] == '<')
		if (tmp->cmd_segment[i] != '>')
			return (0);
	if (tmp->cmd_segment[0] == '>')
		while (tmp->cmd_segment[++i])
			if (tmp->cmd_segment[i] != '>')
				return (-1);
	if (i > 2)
		return (-1);
	if (tmp->cmd_segment[0] == '<')
		while (tmp->cmd_segment[++i])
			if (tmp->cmd_segment[i] != '<')
				return (-1);
	if (i > 2)
		return (-1);
	return (0);
}

/*
	Utilise check_redir_error pour valider le token, et identifie le type de
	redirection (sortie, entrée, append, etc.) basé sur le contenu du token.
*/
t_token	which_redir(t_lexer *tmp)
{
	if (!check_redir_error(tmp))
	{
		if (tmp->cmd_segment[0] == '>' && tmp->cmd_segment[1] == '\0')
			return (REDIRECT_OUT);
		else if (tmp->cmd_segment[0] == '<' && tmp->cmd_segment[1] == '\0')
			return (REDIRECT_IN);
		else if (tmp->cmd_segment[0] == '>' && tmp->cmd_segment[1] == '>')
			return (APPEND_OUT);
		else if (tmp->cmd_segment[0] == '<' && tmp->cmd_segment[1] == '<')
			return (HERE_DOC);
		else if (tmp->cmd_segment[0] == '<' && tmp->cmd_segment[1] == '>')
			return (REDIRECT_INOUT);
		else if (tmp->cmd_segment[0] == '>' && tmp->cmd_segment[1] == '<')
			return (REDIRECT_OUTIN);
	}
	return (-1);
}

/*
	Attribue un token CMD si le premier token n'est pas une commande,
	sinon attribue ARG.
*/
void	assign_command_or_argument_token(t_lexer *tmp, t_lexer *first)
{
	if (first->token != CMD)
		tmp->token = CMD;
	else
		tmp->token = ARG;
}

/*
	Parcourt la chaîne en comptant les mots, délimités par des espaces.
*/
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
