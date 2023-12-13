
#include "../../include/minishell.h"

/**
 * @nom: ft_read_input
 *
 * @description: Lit l'entrée utilisateur pour un here-document dans un shell. Cette fonction est 
 *               essentielle pour lire des entrées multi-lignes jusqu'à un délimiteur spécifique,
 *               souvent utilisé dans les scripts shell et les commandes interactives.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande concerné.
 *   - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes indiquant le délimiteur.
 *
 * @fonctionnement:
 *   - Duplique le descripteur d'entrée standard pour préserver l'état original.
 *   - Entre dans une boucle infinie pour lire les lignes d'entrée une par une.
 *   - Si `readline` retourne NULL et `can_run` est actif, signale la fin du here-document.
 *   - En cas d'interruption (flag `can_run` désactivé), restaure le descripteur d'entrée standard et sort de la boucle.
 *   - Vérifie si la ligne lue correspond au délimiteur spécifié pour terminer la lecture.
 *   - Écrit chaque ligne lue dans un fichier temporaire, suivi d'un saut de ligne.
 *   - Libère la mémoire allouée pour chaque ligne lue après l'écriture.
 *   - À la fin de la lecture, ferme le descripteur dupliqué et libère la dernière ligne lue.
 *
 *   Pourquoi cette fonction est-elle importante ?
 *   - Lecture Multi-Lignes : Permet la saisie d'entrées utilisateur étendues, essentielles pour les here-documents.
 *   - Gestion des Délimiteurs : Reconnaît le délimiteur spécifié pour terminer correctement la lecture du here-document.
 *   - Préservation de l'État d'Entrée : Assure que l'état d'entrée standard est préservé et restauré après la lecture.
 *   - Stockage Temporaire : Écrit les entrées dans un fichier temporaire pour un traitement ultérieur.
 *
 * @valeur_retour: 
 *   Aucune (void). La fonction lit l'entrée et met à jour le fichier temporaire et les utilitaires globaux.
 *
 * @erreurs: 
 *   - Gère les cas où l'entrée est interrompue ou le délimiteur n'est pas trouvé.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lexer_lst = create_lexer_list("<< EOF");
 *   ft_read_input(node, lexer_lst);
 *
 * @dépendances: 
 *   - ft_write_fd pour l'écriture dans le fichier temporaire.
 *   - readline pour la lecture de l'entrée utilisateur.
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
 *   - Ligne lue est-elle NULL et can_run activé ?
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Écrire un avertissement (ERR_HEREDOC_EOF_WARNING)   - can_run est-il désactivé ?
 *   - Sortir de la boucle                                /       \
 *                                                         OUI      NON
 *                                                          |         |
 *                                                          v         v
 *   - Restaurer le descripteur d'entrée standard (dup2)   - La ligne correspond-elle au délimiteur ?
 *   - Sortir de la boucle                                 /       \
 *                                                         OUI      NON
 *                                                          |         |
 *                                                          v         v
 *   - Sortir de la boucle                                 - Écrire la ligne dans le fichier temporaire (ft_write_fd)
 *                                                          - Écrire un saut de ligne (FT_NEWLINE)
 *                                                          - Libérer la mémoire de la ligne lue
 *                                                          - Lire la ligne suivante
 *   |                                                     |
 *   |-----------------------------------------------------|
 *   v
 *   - Fermer le descripteur d'entrée standard dupliqué (close)
 *   - Libérer la mémoire de la dernière ligne lue
 *   |
 *   v
 *   Fin
 */
void	ft_read_input(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->dupin = dup(0);
	while (INFINITY_LOOP)
	{
		data->utils->hd = readline("> ");
		if (!data->utils->hd && data->utils->can_run)
		{
			write (STDERR_FILENO, ERR_HEREDOC_EOF_WARNING, ft_strlen(ERR_HEREDOC_EOF_WARNING));
			break ;
		}
		if (!data->utils->can_run)
		{
			dup2(data->utils->dupin, STDIN_FILENO);
			break ;
		}
		if (!ft_strncmp(lexer_lst->next->word, data->utils->hd, \
		strlen2(lexer_lst->next->word)))
			if (strlen2(lexer_lst->next->word) == strlen2(data->utils->hd))
				break ;
		ft_write_fd(data->utils->hd, node->here_doc_fd);
		ft_write_fd("\n", node->here_doc_fd);
		free(data->utils->hd);
	}	
	close(data->utils->dupin);
	free(data->utils->hd);
}

/**
 * @nom: manage_here_doc_process
 *
 * @description: Gère le processus de création d'un here-document pour un nœud de commande dans un shell.
 *               Ce processus est essentiel pour permettre la lecture d'entrées multi-lignes jusqu'à un
 *               délimiteur spécifique.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande concerné.
 *   - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes.
 *
 * @fonctionnement:
 *   - Active le mode here-document en définissant le flag `is_here_doc`.
 *   - Configure les gestionnaires de signaux pour le here-document en utilisant `handle_sig`.
 *   - Ouvre un fichier temporaire pour stocker les entrées du here-document.
 *   - Utilise `ft_read_input` pour lire les lignes d'entrée utilisateur et les écrire dans le fichier temporaire.
 *   - Réinitialise les gestionnaires de signaux à leur état original après la lecture.
 *   - Ferme le fichier temporaire une fois la lecture terminée.
 *
 *   Pourquoi cette fonction est-elle importante ?
 *   - Lecture Multi-Lignes : Permet de lire des entrées utilisateur sur plusieurs lignes, ce qui est crucial
 *     pour les scripts et commandes qui nécessitent des entrées étendues.
 *   - Flexibilité d'Entrée : Offre une méthode flexible pour fournir des données complexes à un programme ou une commande.
 *   - Gestion des Signaux : Assure que les signaux comme Ctrl-C sont gérés adéquatement pendant la saisie du here-document.
 *
 * @valeur_retour: 
 *   Aucune (void). La fonction effectue des opérations et met à jour le nœud et les utilitaires globaux.
 *
 * @erreurs: 
 *   - Gère les erreurs d'ouverture de fichier et de lecture d'entrée, modifiant les états d'erreur au besoin.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lexer_lst = create_lexer_list("<< EOF");
 *   manage_here_doc_process(node, lexer_lst);
 *
 * @dépendances: 
 *   - ft_read_input pour lire l'entrée utilisateur.
 *   - handle_sig pour configurer les gestionnaires de signaux.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Activer le mode here-document (is_here_doc = 1)
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
 *   - Désactiver le mode here-document (is_here_doc = 0)
 *   |           |
 *   v           v
 *   Fin         Fin
 */
void	manage_here_doc_process(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	data->utils->is_here_doc = 1;
	handle_sig(data);
	node->here_doc = HEREDOC_TEMP_FILE;
	node->here_doc_fd = open(node->here_doc, HEREDOC_TMPFILE_FLAGS, PERM_RWX_ALL);
	if (node->here_doc_fd < 0)
		return ;
	ft_read_input(node, lexer_lst, data);
	data->utils->is_here_doc = 0;
	handle_sig(data);
	close(node->here_doc_fd);
}
