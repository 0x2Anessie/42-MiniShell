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
// void	ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data)
// {
// 	data->utils->stdin_fd_for_heredoc = dup(0);
// 	while (INFINITY_LOOP)
// 	{
// 		data->utils->heredoc_input_buffer = readline("> ");
// 		if (is_heredoc_ended_by_signal(data))
// 		{
// 			dup2(data->utils->stdin_fd_for_heredoc, STDIN_FILENO);
// 			break ;
// 		}
// 		if (is_heredoc_delimiter_matched(\
// 		lexer_lst->next->cmd_segment, data->utils->heredoc_input_buffer))
// 			break ;
// 		write_line_to_heredoc(\
// 		data->utils->heredoc_input_buffer, node->here_doc_fd);
// 		free(data->utils->heredoc_input_buffer);
// 	}
// 	close(data->utils->stdin_fd_for_heredoc);
// 	free(data->utils->heredoc_input_buffer);
// }

void	write_line_to_heredoc(char *line, int heredoc_fd)
{
	ft_write_fd(line, heredoc_fd);
	ft_write_fd("\n", heredoc_fd);
}

bool    is_valid_variable_char(char c)
{
	return isalnum(c) || c == '_'; // Les noms de variables peuvent inclure des lettres, des chiffres et des underscores
}

char*   get_variable_value(char *var_name, t_data *data)
{
	if (!var_name || !data)
		return NULL;
	for (int i = 0; data->full_env_var_copy_alpha[i] != NULL; i++)
	{
		char *env_entry = data->full_env_var_copy_alpha[i];
		char *separator = ft_strchr(env_entry, '=');
		if (!separator)
			continue;
		int name_length = separator - env_entry;
		if (ft_strncmp(var_name, env_entry, name_length) == 0 && strlen2(var_name) == name_length)
			return separator + 1;
	}
	return NULL;
}

int is_backslash_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '\\')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
}

void    remove_escape_character(char **line, int index)
{
	// Décale le contenu de la chaîne vers la gauche pour supprimer le caractère d'échappement.
	// N'oubliez pas de réajuster la fin de la chaîne avec un caractère nul.
	int i;
	for (i = index; (*line)[i]; i++)
	{
		(*line)[i] = (*line)[i + 1];
	}
	(*line)[i] = '\0'; // S'assurer que la chaîne est correctement terminée.
}

/**
 * Vérifie si le caractère à l'index donné doit être échappé.
 */
int is_escaped(char *line, int index)
{
	return (index > 0 && line[index - 1] == '\\');
}

void    expand_variable(char **line, int index, t_data *data)
{
	// Extrait le nom de la variable après le signe '$'
	int var_name_start = index + 1;
	int var_name_length = 0;

	while ((*line)[var_name_start + var_name_length] &&
		   is_valid_variable_char((*line)[var_name_start + var_name_length]))
	{
		var_name_length++;
	}

	if (var_name_length == 0)
	{
		return; // Pas de nom de variable valide après '$'
	}

	// Copie le nom de la variable
	char var_name[var_name_length + 1];
	strncpy(var_name, *line + var_name_start, var_name_length);
	var_name[var_name_length] = '\0';

	// Obtient la valeur de la variable
	char *var_value = get_variable_value(var_name, data); // Implémentez cette fonction

	if (!var_value)
	{
		var_value = ""; // Si la variable n'est pas trouvée, utilisez une chaîne vide
	}

	// Construit la nouvelle ligne
	int new_line_length = strlen(*line) - var_name_length - 1 + strlen(var_value);
	char *new_line = malloc(new_line_length + 1);
	if (!new_line) {
		return; // Gestion d'erreur d'allocation mémoire
	}

	// Copie le début de la ligne
	strncpy(new_line, *line, index);
	// Copie la valeur de la variable
	strcpy(new_line + index, var_value);
	// Copie le reste de la ligne
	strcpy(new_line + index + strlen(var_value), *line + var_name_start + var_name_length);

	free(*line);
	*line = new_line;
}

/**
Traite une ligne lue dans un heredoc pour l'expansion des variables et la gestion des échappements.
*/
void    process_heredoc_line(char **line, t_data *data)
{
	for (int i = 0; (*line)[i]; ++i)
	{
		if ((*line)[i] == '$' && !is_escaped(*line, i))
		{
			// Expansion de la variable
			expand_variable(line, i, data);
		}
		else if (is_escaped(*line, i))
		{
			// Suppression du caractère d'échappement
			remove_escape_character(line, i);
		}
	}
}

void    ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->heredoc_nbr = 1;
	data->utils->stdin_fd_for_heredoc = dup(0);
	while (INFINITY_LOOP)
	{
		char *line = readline("> ");
		if (line == NULL && data->utils->heredoc_ctrl_c_uninterrupted)
		{
			dup2(data->utils->stdin_fd_for_heredoc, STDIN_FILENO);
			break;
		}
		if (is_heredoc_delimiter_matched(lexer_lst->next->cmd_segment, line))
		{
			free(line);
			break;
		}
		process_heredoc_line(&line, data);
		write_line_to_heredoc(line, node->here_doc_fd);
		free(line);
	}
	close(data->utils->stdin_fd_for_heredoc);
	free(data->utils->heredoc_input_buffer);
}

/**
 * @nom: manage_here_doc_process
 *
 * @description:
 * Gère le processus de création d'un here-document pour un nœud de commande
 * dans un shell. Ce processus est essentiel pour permettre la lecture
 * d'entrées multi-lignes jusqu'à un délimiteur spécifique.
 *
 * @parametres:
 *   - node: t_node *node, pointeur vers le nœud de commande concerné.
 *   - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes.
 *
 * @fonctionnement:
 * - Active le mode here-document en définissant le flag `in_here_doc_mode`.
 * - Configure les gestionnaires de signaux pour le here-document en
 * utilisant `handle_sig`.
 * - Ouvre un fichier temporaire pour stocker les entrées du here-document.
 * - Utilise `ft_read_input` pour lire les lignes d'entrée utilisateur et les
 * écrire dans le fichier temporaire.
 * - Réinitialise les gestionnaires de signaux à leur état original après la
 * lecture.
 * - Ferme le fichier temporaire une fois la lecture terminée.
 *
 * Pourquoi cette fonction est-elle importante ?
 * - Lecture Multi-Lignes : Permet de lire des entrées utilisateur sur
 * plusieurs lignes, ce qui est crucial
 * pour les scripts et commandes qui nécessitent des entrées étendues.
 * - Flexibilité d'Entrée : Offre une méthode flexible pour fournir des
 * données complexes à un programme ou une commande.
 * - Gestion des Signaux : Assure que les signaux comme Ctrl-C sont gérés
 * adéquatement pendant la saisie du here-document.
 *
 * @valeur_retour:
 * Aucune (void). La fonction effectue des opérations et met à jour le nœud et
 * les utilitaires globaux.
 *
 * @erreurs:
 * - Gère les erreurs d'ouverture de fichier et de lecture d'entrée, modifiant
 * les états d'erreur au besoin.
 *
 * @exemple_utilisation:
 * t_node *node = create_node();
 * t_lexer *lexer_lst = create_lexer_list("<< EOF");
 * manage_here_doc_process(node, lexer_lst);
 *
 * @dependances:
 *   - ft_read_input pour lire l'entrée utilisateur.
 *   - handle_sig pour configurer les gestionnaires de signaux.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Activer le mode here-document (in_here_doc_mode = 1)
 *   |
 *   v
 *   - Configurer les gestionnaires de signaux (handle_sig)
 *   |
 *   v
 *   - Ouvrir un fichier temporaire pour le here-document (HEREDOC_TEMP_FILE)
 *   |
 *   v
 *   - Fichier temporaire ouvert avec succès ?
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Retourner  Lire les entrées utilisateur (ft_read_input)
 *     sans rien  |
 *     faire      v
 *   - Écrire les entrées dans le fichier temporaire
 *   |           |
 *   v           v
 *   - Fin de la lecture (délimiteur atteint ou interruption)
 *   |           |
 *   v           v
 *   - Réinitialiser les gestionnaires de signaux (handle_sig)
 *   |           |
 *   v           v
 *   - Fermer le fichier temporaire
 *   |           |
 *   v           v
 *   - Désactiver le mode here-document (in_here_doc_mode = 0)
 *   |           |
 *   v           v
 *   Fin         Fin
 */
void	manage_here_doc_process(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->is_this_an_exec_in_heredoc = TRUE;
	handle_sig(data);
	node->heredoc_tmp_fullname = HEREDOC_TEMP_FILE;
	node->here_doc_fd = open(\
	node->heredoc_tmp_fullname, heredoc_tmp_file_flags(), PERM_RWX_ALL);
	if (is_heredoc_file_opening_failed(node->here_doc_fd))
		return ;
	ft_read_input(node, lexer_lst, data);
	data->utils->is_this_an_exec_in_heredoc = FALSE;
	handle_sig(data);
	close(node->here_doc_fd);
}
