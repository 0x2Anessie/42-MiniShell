
#include "../../include/minishell.h"

/**
 * @nom: configure_here_doc_input
 *
 * @description: Configure un here-doc comme entrée pour un nœud de commande, basée
 *               sur une liste de lexèmes. Cette fonction est essentielle pour gérer
 *               les entrées multi-lignes dans un shell, permettant une entrée interactive
 *               ou scriptée.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande.
 *   - lex_lst: t_lexer *lex_lst, pointeur vers le début de la liste de lexèmes.
 *
 * @fonctionnement:
 *   Vérifie si les lexèmes indiquent un here-doc (HERE_DOC et DELIMITER). Si oui, 
 *   ferme l'entrée existante, puis appelle `manage_here_doc_process` pour créer
 *   le here-doc. Ouvre ensuite le fichier temporaire du here-doc et le configure
 *   comme entrée du nœud. Gère les cas d'échec en mettant à jour l'indicateur d'entrée
 *   et en supprimant le fichier temporaire.
 *   - `ft_read_input` lit l'entrée utilisateur pour le here-doc.
 *   - `handle_sig` configure les signaux pendant la lecture du here-doc.
 *   - `manage_here_doc_process` orchestre la création du here-doc.
 *
 *   Pourquoi configurer un here-doc ?
 *   - Entrée multi-lignes : Fournit une méthode pour saisir des entrées sur plusieurs lignes,
 *     utile pour les scripts et les commandes interactives.
 *   - Flexibilité d'entrée : Permet aux utilisateurs de fournir des données complexes
 *     ou volumineuses de manière interactive.
 *   - Gestion des signaux : Assure que le processus de saisie du here-doc est interrompu
 *     proprement en cas de signaux comme CTRL-C.
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
 * @dépendances: 
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
 *  |     Mettre à jour node->in
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
void	configure_here_doc_input(t_node *node, t_lexer *lex_lst)
{
	if (lex_lst && lex_lst->token == HERE_DOC \
	&& lex_lst->next->token == DELIMITER)
	{
		if (node->in > 0)
			close (node->in);
		manage_here_doc_process(node, lex_lst);
		if (!access(node->here_doc, F_OK))
		{
			node->in = open(node->here_doc, O_RDONLY);
			unlink(node->here_doc);
		}
		else
		{
			node->in = -2;
			unlink(node->here_doc);
			perror(node->here_doc);
		}
	}
}

/**
 * @nom: setup_input_redirection
 *
 * @description: Configure la redirection d'entrée pour un nœud de commande,
 *               basée sur une liste de lexèmes. Cette configuration est fondamentale
 *               dans un shell pour diriger l'entrée des commandes depuis des fichiers
 *               ou d'autres sources, comme les here-docs.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande à configurer.
 *   - lexer_lst: t_lexer *lexer_lst, pointeur vers le début de la liste de lexèmes.
 *
 * @fonctionnement:
 *   Initialise les variables de redirection d'entrée dans le nœud (`in`, `in_fail`).
 *   Parcourt la liste de lexèmes pour configurer les redirections d'entrée normales
 *   et les here-docs. Utilise `configure_here_doc_input` pour gérer les here-docs
 *   et `get_in_fail` en cas d'échec de redirection d'entrée.
 *
 *   Pourquoi configurer la redirection d'entrée ?
 *   - Contrôle des données entrantes : Oriente l'entrée des commandes depuis des fichiers
 *     ou des sources spécifiques, permettant une plus grande flexibilité dans l'exécution des commandes.
 *   - Gestion des here-docs : Permet d'utiliser des here-docs pour fournir des entrées
 *     multi-lignes, une fonctionnalité importante dans les scripts shell et les commandes interactives.
 *   - Gestion des erreurs : Détecte et gère les problèmes de redirection pour éviter les
 *     comportements imprévus et les erreurs silencieuses.
 *
 * @valeur_retour: 
 *   Aucune (void). Modifie le nœud de commande en place.
 *
 * @erreurs: 
 *   - Écrit un message d'erreur si l'ouverture du fichier ou la configuration du here-doc échoue.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lexer_lst = create_lexer_list("commande < fichier.txt");
 *   setup_input_redirection(node, lexer_lst);
 *
 * @dépendances: 
 *   - configure_here_doc_input pour les here-docs.
 *   - get_in_fail pour la gestion des erreurs.
 *   - ft_write_fd pour l'écriture des messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialisation des champs de redirection d'entrée dans node
 *     |
 *     v
 *   Parcourir lexer_lst
 *     |
 *     v
 *   lexer_lst est-il NULL ou token == PIPE ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Redirection d'entrée normale ou here-doc ?
 *  |    /         \
 *  |  NORMAL    HERE-DOC
 *  |    |          |
 *  |    v          v
 *  |  Gérer     Gérer here-doc
 *  |  avec      avec configure_here_doc_input
 *  |  get_in_fail
 *  |    |
 *  \----/
 *     |
 *     v
 *   Passer au lexème suivant
 *     |
 *     v
 *   Fin
 */
void	setup_input_redirection(t_node *node, t_lexer *lexer_lst)
{
	node->in = -1;
	node->in_fail = 0;
	while (lexer_lst && lexer_lst->token != PIPE)
	{
		if (lexer_lst && lexer_lst->token == flecheg \
		&& lexer_lst->next->token == FD)
		{
			if (node->in > 0)
				close(node->in);
			if (!lexer_lst->next->word)
				node->in_fail = 1;
			if (lexer_lst->next->word && !access(lexer_lst->next->word, R_OK))
				node->in = open(lexer_lst->next->word, O_RDONLY);
			else
				get_in_fail(node, lexer_lst);
		}
		configure_here_doc_input(node, lexer_lst);
		lexer_lst = lexer_lst->next;
	}
}

/**
 * @nom: Dflechedput_redirection
 *
 * @description: Configure une redirection de sortie de type append pour un nœud de 
 *               commande, en fonction des lexèmes fournis. Cette opération est essentielle
 *               pour ajouter des sorties à la fin d'un fichier existant, sans écraser
 *               son contenu actuel, dans un contexte de shell.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande à configurer.
 *   - lex_lst: t_lexer *lex_lst, pointeur vers la liste de lexèmes.
 *   - has_out: int *has_out, pointeur vers l'indicateur de présence de redirection de sortie.
 *
 * @fonctionnement:
 *   Vérifie si les lexèmes indiquent une redirection de sortie de type append (Dfleched).
 *   Si oui, ferme la sortie existante si ouverte, puis tente d'ouvrir le fichier de 
 *   redirection en mode append. En cas d'échec d'ouverture ou d'absence de nom de fichier,
 *   écrit un message d'erreur et met à jour l'indicateur d'échec (`out_fail`).
 *   - `ft_write_fd` est utilisée pour écrire les messages d'erreur.
 *   - Gère également les cas d'échec d'accès au fichier.
 *
 *   Pourquoi gérer la redirection de sortie de type append ?
 *   - Ajout de données : Permet d'ajouter des sorties à un fichier existant sans 
 *     écraser son contenu, ce qui est crucial pour les journaux ou la concaténation de données.
 *   - Précision de la redirection : Fournit un contrôle plus fin sur la manière dont les 
 *     données sont ajoutées aux fichiers, en respectant le contenu préexistant.
 *   - Flexibilité dans l'exécution des commandes : Offre la possibilité de manipuler la 
 *     sortie des commandes pour des besoins spécifiques, comme l'ajout de logs ou la 
 *     collecte de données sur une période.
 *
 * @valeur_retour: 
 *   Aucune (void). Modifie le nœud de commande en place.
 *
 * @erreurs: 
 *   - Écrit un message d'erreur si l'ouverture du fichier échoue ou si l'accès est refusé.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lex_lst = create_lexer_list("commande >> fichier.txt");
 *   Dflechedput_redirection(node, lex_lst, &node->has_out);
 *
 * @dépendances: 
 *   - ft_write_fd pour écrire les messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la présence d'une redirection de sortie append
 *     |
 *     v
 *   Condition de redirection append remplie ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Fin de la fonction
 *  |
 *  v
 * Fermer la sortie existante si ouverte
 *  |
 *  v
 * Ouvrir le fichier de redirection en mode append
 *  |
 *  v
 * Ouverture réussie ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Écrire message d'erreur avec ft_write_fd
 *  |        |
 *  |        v
 *  |     Mettre à jour node->out_fail
 *  |
 *  v
 * Accès en écriture refusé ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Mettre à jour node->out_fail
 *  | 
 *  v
 * Mettre à jour *has_out
 *  |
 *  v
 * Fin
 */
void	Dflechedput_redirection(t_node *node, t_lexer *lex_lst, int *has_out)
{
	if (lex_lst && lex_lst->next && lex_lst->token == Dfleched \
	&& lex_lst->next->token == FD)
	{
		if (node->out > 0)
			close (node->out);
		if (lex_lst->next->word)
			node->out = open(lex_lst->next->word, APPND_TO_FILE_FLAGS, PERM_O_RW_G_R_OT_R);
		else
		{
			ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
			node->out_fail = 2;
		}
		if (node->out_fail != 2 && (node->out == -1
				|| !access(lex_lst->next->word, F_OK)))
			node->out_fail = 1;
		*has_out = 1;
	}
}

/**
 * @nom: normal_output_redirection
 *
 * @description: Configure une redirection de sortie standard pour un nœud de commande
 *               en se basant sur une liste de lexèmes. Cette opération est cruciale
 *               pour diriger la sortie des commandes vers des fichiers ou d'autres
 *               processus dans un environnement de shell.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande à configurer.
 *   - lex_lst: t_lexer *lex_lst, pointeur vers le début de la liste de lexèmes.
 *
 * @fonctionnement:
 *   Vérifie si les lexèmes actuels indiquent une redirection de sortie standard (fleched).
 *   Si oui, ferme la sortie existante si elle est ouverte, puis tente d'ouvrir le fichier
 *   de redirection spécifié. Si l'ouverture échoue ou si le nom du fichier n'est pas donné,
 *   écrit un message d'erreur et met à jour l'indicateur d'échec de sortie (`out_fail`).
 *
 *   Pourquoi gérer la redirection de sortie standard ?
 *   - Contrôle de flux : Permet de rediriger la sortie des commandes vers des fichiers,
 *     ce qui est essentiel pour enregistrer des résultats ou chaîner des commandes.
 *   - Gestion des erreurs : Assure que les échecs de redirection sont détectés et traités,
 *     évitant ainsi les comportements imprévus et les erreurs silencieuses.
 *   - Flexibilité dans l'exécution des commandes : Offre la possibilité de manipuler la
 *     sortie des commandes pour des besoins spécifiques, comme la création de logs.
 *
 * @valeur_retour: 
 *   Aucune (void). Modifie le nœud de commande en place.
 *
 * @erreurs: 
 *   - Écrit un message d'erreur si l'ouverture du fichier échoue ou si l'accès est refusé.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lex_lst = create_lexer_list("commande > fichier.txt");
 *   normal_output_redirection(node, lex_lst);
 *
 * @dépendances: 
 *   - ft_write_fd pour écrire les messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la présence d'une redirection de sortie standard
 *     |
 *     v
 *   Condition de redirection standard remplie ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Fin de la fonction
 *  |
 *  v
 * Fermer la sortie existante si ouverte
 *  |
 *  v
 * Ouvrir le fichier de redirection
 *  |
 *  v
 * Ouverture réussie ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Écrire message d'erreur avec ft_write_fd
 *  |        |
 *  |        v
 *  |     Mettre à jour node->out_fail
 *  |
 *  v
 * Accès en écriture refusé ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Mettre à jour node->out_fail
 *  | 
 *  v
 * Mettre à jour node->has_out
 *  |
 *  v
 * Fin
 */
void	normal_output_redirection(t_node *node, t_lexer *lex_lst)
{
	if (lex_lst && lex_lst->next && lex_lst->token == fleched \
	&& lex_lst->next->token == FD)
	{
		if (node->out > 0)
			close (node->out);
		if (lex_lst->next->word)
			node->out = open(lex_lst->next->word, OUT_TO_FILE_FLAGS, PERM_O_RW_G_R_OT_R);
		else
		{
			ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
			node->out_fail = 2;
		}
		if (node->out_fail != 2 && (node->out == -1
				|| !access(lex_lst->next->word, W_OK)))
			node->out_fail = 1;
		node->has_out = 1;
	}
}

/**
 * @nom: setup_output_redirection
 *
 * @description: Configure la redirection de sortie pour un nœud de commande
 *               en utilisant une liste de lexèmes.
 *
 * @paramètres:
 *   - node: t_node *node, pointeur vers le nœud de commande.
 *   - lex_lst: t_lexer *lex_lst, pointeur vers le début de la liste de lexèmes.
 *
 * @fonctionnement:
 *   Initialise les champs de redirection de sortie du nœud (`has_out`, `out`, `out_fail`).
 *   Parcourt la liste de lexèmes, appelant `normal_output_redirection` et 
 *   `Dflechedput_redirection` pour chaque lexème. Si une redirection est présente
 *   mais échoue, un message d'erreur est généré et les indicateurs de redirection
 *   sont réinitialisés. La boucle se poursuit jusqu'à la rencontre d'un token PIPE
 *   ou la fin de la liste.
 *   - `normal_output_redirection` gère les redirections de sortie normales.
 *   - `Dflechedput_redirection` gère les redirections de sortie de type append.
 *
  * Pourquoi cette fonction est-elle importante ?
 *   La fonction `setup_output_redirection` est essentielle dans un environnement de shell 
 *   pour rediriger la sortie des commandes vers des fichiers ou d'autres processus. Cette 
 *   capacité de redirection offre une flexibilité cruciale pour le contrôle des données 
 *   générées par les commandes. Voici quelques points clés :
 *   
 *   - **Contrôle de la Sortie des Commandes** : La redirection de sortie permet aux 
 *     utilisateurs de diriger les données générées par les commandes vers des fichiers ou 
 *     d'autres destinations, augmentant ainsi les options pour manipuler et enregistrer les 
 *     sorties des commandes.
 *   - **Gestion des Redirections Multiples** : La fonction gère à la fois les redirections 
 *     normales (écrasement) et les redirections de type append (ajout), donnant aux 
 *     utilisateurs la flexibilité de choisir comment gérer la sortie des commandes en 
 *     fonction de leurs besoins.
 *   - **Robustesse et Fiabilité** : En traitant les erreurs de redirection et en réinitialisant 
 *     les indicateurs appropriés, la fonction assure que le shell reste stable et fiable, 
 *     même dans des situations où les redirections échouent ou sont mal configurées.
 * 
 * @valeur_retour: 
 *   Aucune (void). Modifie le nœud de commande en place.
 *
 * @erreurs: 
 *   - Génère un message d'erreur si la redirection de sortie échoue.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lex_lst = create_lexer_list("cmd > file.txt | cmd2");
 *   setup_output_redirection(node, lex_lst);
 *
 * @dépendances: 
 *   - normal_output_redirection
 *   - Dflechedput_redirection
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialisation des champs de redirection dans node
 *     |
 *     v
 *   Parcourir lex_lst
 *     |
 *     v
 *   lex_lst est-il NULL ou token == PIPE ?
 *  /       \
 * OUI      NON
 *  |        |
 *  |        v
 *  |     Appeler normal_output_redirection
 *  |        |
 *  |        v
 *  |     Appeler Dflechedput_redirection
 *  |        |
 *  |        v
 *  |     Redirection échouée ?
 *  |    /       \
 *  |   OUI      NON
 *  |    |        |
 *  |    v        v
 *  |  Générer  Passer au lexème suivant
 *  |  message d'erreur
 *  |    |
 *  |    v
 *  |  Réinitialiser les indicateurs de redirection
 *  |    |
 *  \----/
 *     v
 *   Fin
 */
void	setup_output_redirection(t_node *node, t_lexer *lex_lst)
{
	node->has_out = 0;
	node->out = -1;
	node->out_fail = 0;
	while (lex_lst && lex_lst->token != PIPE)
	{
		normal_output_redirection(node, lex_lst);
		Dflechedput_redirection(node, lex_lst, &node->has_out);
		if (node->has_out && node->out == -1
			&& node->in != -2 && node->out_fail != 2)
		{
			node->has_out = 0;
			perror(OUT_FILE);
		}
		lex_lst = lex_lst->next;
	}
}
