#include "../../include/minishell.h"

void	write_line_to_heredoc(char *line, int heredoc_fd)
{
	ft_write_fd(line, heredoc_fd);
	ft_write_fd("\n", heredoc_fd);
}

bool	is_heredoc_delimiter_matched(char *delimiter, char *line)
{
    return (!ft_strncmp(delimiter, line, strlen2(delimiter)) \
	&& (strlen2(delimiter) == strlen2(line)));
}


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
 * - Si `readline` retourne NULL et `heredoc_ctrl_c_uninterrupted` est actif, signale la fin du
 * here-document.
 * - En cas d'interruption (flag `heredoc_ctrl_c_uninterrupted` désactivé), restaure le
 * descripteur d'entrée standard et sort de la boucle.
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
 *   - Écrire un avertissement          - heredoc_ctrl_c_uninterrupted est-il désactivé ?
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
void	ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->stdin_fd_for_heredoc = dup(0);
	while (INFINITY_LOOP)
	{
		data->utils->heredoc_input_buffer = readline("> ");
		// printf("valeur de line : %s\n", data->utils->heredoc_input_buffer);
		if (!data->utils->heredoc_input_buffer && data->utils->heredoc_ctrl_c_uninterrupted)/*         ---> condition non intelligible --> fonction         */
		{
			write (\
			STDERR_FILENO, ERR_HEREDOC_EOF_WARNING, \
			ft_strlen(ERR_HEREDOC_EOF_WARNING));
			break ;
		}
		if (!data->utils->heredoc_ctrl_c_uninterrupted)/*         ---> condition non intelligible --> fonction         */
		{
			dup2(data->utils->stdin_fd_for_heredoc, STDIN_FILENO);
			break ;
		}
		// printf("valeur de line : %s\n", data->utils->heredoc_input_buffer);
		// printf("valeur de word : %s\n", lexer_lst->next->word);
		if (is_heredoc_delimiter_matched(lexer_lst->next->word, data->utils->heredoc_input_buffer))
				break ;
		write_line_to_heredoc(data->utils->heredoc_input_buffer, node->here_doc_fd);
		free(data->utils->heredoc_input_buffer);
	}
	close(data->utils->stdin_fd_for_heredoc);
	free(data->utils->heredoc_input_buffer);
}

bool	is_heredoc_file_opening_failed(int file_descriptor)
{
    return (file_descriptor < 0);
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
