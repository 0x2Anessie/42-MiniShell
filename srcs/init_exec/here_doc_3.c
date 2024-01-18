#include "../../include/minishell.h"

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
