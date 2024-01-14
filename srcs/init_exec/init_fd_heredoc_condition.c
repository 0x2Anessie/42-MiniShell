#include "../../include/minishell.h"

bool	is_here_doc_followed_by_delimiter(t_lexer *lexer_lst)
{
	return (lexer_lst \
	&& lexer_lst->token == HERE_DOC \
	&& lexer_lst->next \
	&& lexer_lst->next->token == DELIMITER);
}

bool	is_heredoc_tmp_file_exists(t_node *node)
{
	return (!access(node->heredoc_tmp_fullname, F_OK));
}

/**
 * @nom: configure_here_doc_input
 *
 * @description:
 * Configure un here-doc comme entrée pour un nœud de commande, basée sur une
 * liste de lexèmes. Cette fonction est essentielle pour gérer les entrées
 * multi-lignes dans un shell, permettant une entrée interactive ou scriptée.
 *
 * @parametres:
 * - node: t_node *node, pointeur vers le nœud de commande.
 * - lex_lst: t_lexer *lex_lst, pointeur vers le début de la liste de lexèmes.
 *
 * @fonctionnement:
 * Vérifie si les lexèmes indiquent un here-doc (HERE_DOC et DELIMITER).
 * Si oui, ferme l'entrée existante, puis appelle `manage_here_doc_process`
 * pour créer le here-doc. Ouvre ensuite le fichier temporaire du here-doc et
 * le configure comme entrée du nœud. Gère les cas d'échec en mettant à jour
 * l'indicateur d'entrée et en supprimant le fichier temporaire.
 * - `ft_read_input` lit l'entrée utilisateur pour le here-doc.
 * - `handle_sig` configure les signaux pendant la lecture du here-doc.
 * - `manage_here_doc_process` orchestre la création du here-doc.
 *
 * Pourquoi configurer un here-doc ?
 * - Entrée multi-lignes : Fournit une méthode pour saisir des entrées sur
 * plusieurs lignes, utile pour les scripts et les commandes interactives.
 * - Flexibilité d'entrée : Permet aux utilisateurs de fournir des données
 * complexes ou volumineuses de manière interactive.
 * - Gestion des signaux : Assure que le processus de saisie du here-doc est
 * interrompu proprement en cas de signaux comme CTRL-C.
 *
 * @valeur_retour: 
 *   Aucune (void). Modifie le nœud de commande en place.
 *
 * @erreurs: 
 *   - Génère un message d'erreur si l'accès au fichier here-doc échoue.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lex_lst = create_lexer_list("commande << DELIMITER");
 *   configure_here_doc_input(node, lex_lst);
 *
 * @dependances: 
 *   - manage_here_doc_process pour la création et gestion du here-doc.
 *   - ft_read_input pour la lecture de l'entrée du here-doc.
 *   - handle_sig pour la gestion des signaux pendant le here-doc.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la présence d'un here-doc
 *     |
 *     v
 *   Condition de here-doc remplie ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Fin de la fonction
 *  |
 *  v
 * Fermer l'entrée existante si ouverte
 *  |
 *  v
 * Appeler manage_here_doc_process
 *  |
 *  v
 * Accès au fichier here-doc réussi ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Générer message d'erreur
 *  |        |
 *  |        v
 *  |     Mettre à jour node->input_fd
 *  |        |
 *  |        v
 *  |     Supprimer le fichier here-doc
 *  | 
 *  v
 * Ouvrir et configurer le fichier here-doc comme entrée
 *  |
 *  v
 * Fin
 */
void	configure_here_doc_input(t_node *node, t_lexer *lex_lst, t_data *data)
{
	if (is_here_doc_followed_by_delimiter(lex_lst))
	{
		if (is_input_fd_ready_for_read(node))
			close (node->input_fd);
		manage_here_doc_process(node, lex_lst, data);
		if (is_heredoc_tmp_file_exists(node))
		{
			node->input_fd = open(node->heredoc_tmp_fullname, O_RDONLY);
			unlink(node->heredoc_tmp_fullname);
		}
		else
		{
			node->input_fd = INPUT_FD_REDIRECTION_FAIL;
			unlink(node->heredoc_tmp_fullname);
			perror(node->heredoc_tmp_fullname);
		}
	}
}
