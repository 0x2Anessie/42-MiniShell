
#include "../../include/minishell.h"

/*
	parcour la list du lexer pour compter le nombre d'arg ou cmd puis allou un tableau avec
	le nombre d'elem trouver puis appel get_words pour remplir le tableau
	puis une fois pret appel sim_echo3
*/

void	init_echo(t_lexer *lexer_lst, t_data *data)
{
	int			i;
	char		**tab;
	t_lexer		*head;

	i = 0;
	tab = NULL;
	head = lexer_lst;
	while ((lexer_lst) && (lexer_lst->token == CMD || lexer_lst->token == ARG))
	{
		lexer_lst = lexer_lst->next;
		i++;
	}
	tab = ft_malloc_with_tracking(data, sizeof(char *) * (i + 1));
	if (!tab)
		return ;
	lexer_lst = head;
	i = 0;
	get_words(lexer_lst, tab, &i);
	tab[i] = NULL;
	simulate_echo(tab);
}

//	verifie si il y a un -n meme avec une ecriture chelou genre -nnnnnnn

int	check_n(char *str)
{
	int	i;

	i = 0;
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
	enfin si nous somme dans aucun des deux cas la fonction ecrit elle meme
	le retour
	a la ligne sur la sorti use
*/

void	simulate_echo(char **tab)
{
	int	i;
	int	ncount;

	i = 0;
	if (tab && tab[i] && tab[i + 1])
		ncount = check_n(tab[i + 1]);
	if (tab[i] && tab[i + 1] && !ft_strcmp(tab[i], "echo")
		&& tab[i + 1][0] != '-' && tab[i + 1][1] != 'n')
		process_echo(tab, ++i);
	else if (tab[i] && tab[i + 1] && !ft_strcmp(tab[i], "echo")
		&& ncount)
		display_echo_arg(tab, i);
	else
	{
		if (g_all.utils->node->out > 0)
			ft_write_fd("\n", g_all.utils->node->out);
		else
			printf("\n");
	}
}
