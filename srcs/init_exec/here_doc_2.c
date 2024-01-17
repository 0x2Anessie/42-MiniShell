#include "../../include/minishell.h"

/**
 * @nom: ft_read_input
 *
 * @description:
 * Lit l'entrée utilisateur pour un here-document dans un shell. Cette
 * fonction est essentielle pour lire des entrées multi-lignes jusqu'à un
 * délimiteur spécifique, souvent utilisé dans les scripts shell et les
 * commandes interactives.
 *
 * @parametres:
 * - node: t_node *node, pointeur vers le nœud de commande concerné.
 * - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes
 * indiquant le délimiteur.
 *
 * @fonctionnement:
 * - Duplique le descripteur d'entrée standard pour préserver l'état
 * original.
 * - Entre dans une boucle infinie pour lire les lignes d'entrée une par une.
 * - Si `readline` retourne NULL et `heredoc_ctrl_c_uninterrupted` est actif,
 * signale la fin du here-document.
 * - En cas d'interruption (flag `heredoc_ctrl_c_uninterrupted` désactivé),
 * restaure le descripteur d'entrée standard et sort de la boucle.
 * - Vérifie si la ligne lue correspond au délimiteur spécifié pour terminer
 * la lecture.
 * - Écrit chaque ligne lue dans un fichier temporaire, suivi d'un saut de
 * ligne.
 * - Libère la mémoire allouée pour chaque ligne lue après l'écriture.
 * - À la fin de la lecture, ferme le descripteur dupliqué et libère la
 * dernière ligne lue.
 *
 * Pourquoi cette fonction est-elle importante ?
 * - Lecture Multi-Lignes : Permet la saisie d'entrées utilisateur étendues,
 * essentielles pour les here-documents.
 * - Gestion des Délimiteurs : Reconnaît le délimiteur spécifié pour terminer
 * correctement la lecture du here-document.
 * - Préservation de l'État d'Entrée : Assure que l'état d'entrée standard
 * est préservé et restauré après la lecture.
 * - Stockage Temporaire : Écrit les entrées dans un fichier temporaire pour
 * un traitement ultérieur.
 *
 * @valeur_retour:
 * Aucune (void). La fonction lit l'entrée et met à jour le fichier
 * temporaire et les utilitaires globaux.
 *
 * @erreurs:
 * - Gère les cas où l'entrée est interrompue ou le délimiteur n'est pas
 * trouvé.
 *
 * @exemple_utilisation:
 * t_node *node = create_node();
 * t_lexer *lexer_lst = create_lexer_list("<< EOF");
 * ft_read_input(node, lexer_lst);
 *
 * @dependances:
 * - ft_write_fd pour l'écriture dans le fichier temporaire.
 * - readline pour la lecture de l'entrée utilisateur.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Dupliquer le descripteur d'entrée standard (dup)
 *   |
 *   v
 *   Entrer dans la boucle de lecture (INFINITY_LOOP)
 *     |
 *     v
 *   - Lire une ligne (readline)
 *     |
 *     v
 *   - Ligne lue est-elle NULL et heredoc_ctrl_c_uninterrupted activé ?
 *                  /             \
 *                OUI             NON
 *                 |               |
 *                 v               v
 *                               - heredoc_ctrl_c_uninterrupted est-il
 *   - Écrire un avertissement            désactivé ?
 *    (ERR_HEREDOC_EOF_WARNING)              /       \
 *   - Sortir de la boucle                  OUI      NON
 *                                           |        |
 *                                           v        v
 *   - Restaurer le descripteur              - La ligne correspond-elle
 *     d'entrée standard (dup2)   			 au délimiteur ?
 *   - Sortir de la boucle                     /         \
 *                                            OUI       NON
 *                                             |         |
 *                                             v         v
 *   - Sortir de la boucle                     - Écrire la ligne dans le
 * 													fichier temporaire
 * 														(ft_write_fd)
 *                                             - Écrire un saut de ligne
 * 													(FT_NEWLINE)
 *                                             - Libérer
 * 													la mémoire de la ligne lue
 *                                             - Lire la ligne suivante
 *   |                                                   |
 *   |---------------------------------------------------|
 *   v
 *   - Fermer le descripteur d'entrée standard dupliqué (close)
 *   - Libérer la mémoire de la dernière ligne lue
 *   |
 *   v
 *   Fin
 */

void	write_line_to_heredoc(char *line, int heredoc_fd)
{
	ft_write_fd(line, heredoc_fd);
	ft_write_fd("\n", heredoc_fd);
}

char	*get_variable_value(char *var_name, t_data *data)
{
	int		i;
	char	*env_entry;
	char	*separator;
	int		name_length;

	i = 0;
	if (!var_name || !data)
		return (NULL);
	while (data->full_env_var_copy_alpha[i] != NULL)
	{
		env_entry = data->full_env_var_copy_alpha[i];
		separator = ft_strchr(env_entry, '=');
		if (!separator)
			continue ;
		name_length = separator - env_entry;
		if (ft_strncmp(var_name, env_entry, name_length) == 0 && \
			strlen2(var_name) == name_length)
			return (separator + 1);
		i++;
	}
	return (NULL);
}

int	is_backslash_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '\\')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
}

void	remove_escape_character(char **line, int index)
{
	int	i;

	i = index;
	while ((*line)[i])
	{
		(*line)[i] = (*line)[i + 1];
		i++;
	}
	(*line)[i] = '\0';
}

/**
 * Vérifie si le caractère à l'index donné doit être échappé.
 */
int	is_escaped(char *line, int index)
{
	return (index > 0 && line[index - 1] == '\\');
}
