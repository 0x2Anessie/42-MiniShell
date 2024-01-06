#include "../../include/minishell.h"

bool	is_first_input_redirection_error(t_node *node)
{
    return (!node->is_input_redirection_failed);
}


/**
 * @nom: handle_redirect_input_error
 *
 * @description:
 * Gère les erreurs de redirection d'entrée pour un nœud de commande.
 * Cette fonction est utilisée pour signaler des erreurs lors de la tentative
 * de redirection d'entrée, comme l'échec de l'ouverture d'un fichier.
 *
 * @parametres:
 * - node: t_node *node, pointeur vers le nœud de commande concerné.
 * - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes actuelle.
 *
 * @fonctionnement:
 * - Examine l'état de `node->is_input_redirection_failed` pour vérifier si une erreur de
 * redirection a déjà été signalée.
 * - Si aucune erreur précédente n'a été signalée, utilise `perror` pour
 * afficher un message d'erreur lié au fichier spécifié dans le lexème.
 * - Si une erreur a déjà été signalée, utilise `ft_write_fd` pour afficher un
 * message d'erreur standardisé indiquant une redirection ambiguë.
 * - Dans les deux cas, met à jour `node->input_fd` à -2 pour indiquer un
 * échec de la redirection d'entrée.
 *
 * Pourquoi cette fonction est-elle importante ?
 * - Gestion des Erreurs : Permet de traiter de manière appropriée les erreurs
 * survenues lors de la configuration des redirections d'entrée, essentielles
 * pour le bon déroulement des commandes dans un shell.
 * - Feedback Utilisateur : Fournit des informations claires à l'utilisateur
 * en cas d'échec de redirection, améliorant ainsi l'expérience utilisateur et
 * la résolution de problèmes.
 * - Intégrité du Processus : Assure que les erreurs sont signalées et gérées
 * correctement, évitant ainsi des comportements inattendus ou des échecs
 * silencieux dans l'exécution des commandes.
 *
 * @valeur_retour: 
 *   Renvoie 1 (int) pour indiquer un échec.
 *
 * @erreurs: 
 *   - Aucune erreur générée par la fonction elle-même, mais signale les
 * erreurs de redirection.
 *
 * @exemple_utilisation:
 *   t_node *node = create_node();
 *   t_lexer *lexer_lst = create_lexer_list("cmd < fichier_inexistant.txt");
 *   handle_redirect_input_error(node, lexer_lst);
 *
 * @dependances: 
 *   - ft_write_fd pour afficher des messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     |
 *     \-------------------->   - Vérifier si
 *                         une erreur de redirection
 *                            a déjà été signalée
 *                              (node->is_input_redirection_failed)
 *                              /             \
 *                            NON            OUI
 *                             |              |
 *                             v              v
 *   - Utiliser perror pour signaler          Afficher un message
 *     l'erreur basée sur					 d'erreur standardisé
 *     le nom du fichier                      (ERR_AMB_REDIRECT)
 *     (lexer_lst->next->word)				   avec ft_write_fd
 *   - Mettre à jour node->input_fd à -2      - Mettre à jour node->input_fd à -2
 *   |                                        |
 *   v                                        v
 *   - Retourner 1 (échec)                    - Retourner 1 (échec)
 *   |                                                 |
 *   v                                                 v
 *   Fin                                              Fin
 */
void	handle_redirect_input_error(t_node *node, t_lexer *lexer_lst)
{
	node->input_fd = INPUT_FD_REDIRECTION_FAIL;
	if (is_first_input_redirection_error(node))
		perror(lexer_lst->next->word);
	else
		ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
}

/**
 * @nom: ft_strdup
 *
 * @description:
 * Duplique une chaîne de caractères en allouant de la mémoire pour la
 * nouvelle copie. Cette fonction est essentielle pour créer des copies
 * indépendantes de chaînes de caractères, souvent utilisées dans les
 * environnements où la manipulation des données originales n'est pas
 * souhaitée ou possible.
 *
 * @parametres:
 *   - src: char *src, pointeur vers la chaîne de caractères à dupliquer.
 *
 * @fonctionnement:
 *   - Commence par vérifier si la chaîne source 'src' n'est pas NULL.
 *   - Calcule la longueur de 'src' pour déterminer la quantité de mémoire
 * nécessaire.
 *   - Utilise 'ft_malloc_with_tracking' pour allouer suffisamment de mémoire
 * pour contenir la 
 *     copie de 'src', y compris le caractère nul de terminaison.
 *   - Copie les caractères de 'src' dans la nouvelle chaîne allouée.
 *   - Retourne un pointeur vers la nouvelle chaîne dupliquée.
 *
 *   Pourquoi utiliser ft_strdup ?
 *   - Création de Copies : Permet de créer des copies de chaînes de
 * caractères qui peuvent être 
 *     modifiées ou conservées indépendamment de l'original.
 *   - Gestion de la Mémoire : Utilise une allocation mémoire dynamique pour
 * s'adapter à la 
 *     taille variable des chaînes, tout en assurant le suivi de ces
 * allocations.
 *   - Sécurité des Données : Évite de modifier les données originales, ce
 * qui peut être 
 *     important pour maintenir l'intégrité des données dans certaines
 * applications.
 *
 * @valeur_retour: 
 *   Renvoie un pointeur vers la nouvelle chaîne de caractères dupliquée.
 * Renvoie NULL si 'src' est NULL ou si l'allocation de mémoire échoue.
 *
 * @erreurs: 
 *   - Retourne NULL si la chaîne source est NULL ou si l'allocation de
 * mémoire échoue.
 *
 * @exemple_utilisation:
 *   char *original = "Hello World";
 *   char *copy = ft_strdup(original);
 *
 * @dependances: 
 *   - ft_malloc_with_tracking pour une allocation mémoire avec suivi.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Vérifier si src est NULL
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Retourner NULL  Calculer la longueur de src
 *                    |
 *                    v
 *   - Allouer de la mémoire pour la duplication (ft_malloc_with_tracking)
 *                    |
 *                    v
 *   - Allocation réussie ?
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Retourner NULL  Copier src dans la nouvelle chaîne
 *                    |
 *                    v
 *   - Ajouter le caractère nul de terminaison
 *                    |
 *                    v
 *   - Retourner la nouvelle chaîne
 *                    |
 *                    v
 *   Fin
 */
char	*ft_strdup(t_data *data, char *src)
{
	int		index;
	char	*dest;

	index = ZERO_INIT;
	if (!src)
		return (NULL);
	while (src[index])
		index++;
	dest = ft_malloc_with_tracking(data, sizeof(char) * index + sizeof('\0'));
	if (!dest)
		return (NULL);
	index = 0;
	while (src[index])
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}

bool	is_current_lexer_token_cmd(t_lexer *current_lexer)
{
    return (current_lexer->token == CMD);
}

/**
 * @nom: is_token_type_cmd
 *
 * @description:
 * Vérifie la présence d'une commande dans une liste de lexèmes. Cette
 * fonction est cruciale pour déterminer si un ensemble de lexèmes représente
 * une commande valide dans un environnement de shell.
 *
 * @parametres:
 *   - lexer_lst: t_lexer *lexer_lst, pointeur vers le début de la liste de
 * lexèmes.
 *
 * @fonctionnement:
 * Parcourt la liste de lexèmes et vérifie si un des lexèmes a le token CMD,
 * indiquant la présence d'une commande. Retourne immédiatement si une
 * commande est trouvée.
 *
 *   Pourquoi vérifier la présence d'une commande ?
 *   - Identification des Commandes : Permet de distinguer entre les entrées
 * qui sont des commandes valides et celles qui ne le sont pas, ce qui est
 * essentiel pour la bonne interprétation et exécution des commandes dans un
 * shell.
 *   - Préparation de l'Exécution : Identifier la présence d'une commande aide
 * à décider si une séquence de lexèmes doit être traitée comme une
 * instruction exécutable.
 *   - Gestion des Erreurs : Contribue à la détection précoce d'erreurs ou de
 * situations non valides, améliorant la robustesse du traitement des
 * commandes.
 *
 * @valeur_retour: 
 * Renvoie CMD_FOUND (int) si une commande est trouvée, sinon CMD_NOT_FOUND
 * (int).
 *
 * @erreurs: 
 * - Aucune erreur générée. La fonction renvoie simplement l'état de la
 * présence d'une commande.
 *
 * @exemple_utilisation:
 *   t_lexer *lexer_lst = create_lexer_list("echo 'Hello World'");
 *   int command_exists = is_token_type_cmd(lexer_lst);
 *
 * @dependances: 
 *   Aucune dépendance externe.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Parcourir la liste de lexèmes
 *     |
 *     v
 *   Token actuel est-il CMD ?
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   Retourner CMD_FOUND  Passer au lexème suivant
 *                        |
 *                        v
 *                      Fin de la liste ?
 *                      /       \
 *                     NON      OUI
 *                      |         |
 *                      v         v
 *                      Retour à la  Retourner CMD_NOT_FOUND
 *                      vérification |
 *                      du token     |
 *                                    v
 *                                   Fin
 */
int	is_token_type_cmd(t_lexer *lexer_lst)
{
	while (lexer_lst)
	{
		if (is_current_lexer_token_cmd(lexer_lst))
			return (CMD_FOUND);
		lexer_lst = lexer_lst->next;
	}
	return (CMD_NOT_FOUND);
}

/**
 * @nom: count_cmd_in_lexer_linked_list
 *
 * @description:
 * Compte le nombre de commandes dans une liste de lexèmes. Cette fonction est
 * essentielle pour déterminer combien de commandes individuelles sont
 * présentes dans une entrée de shell, ce qui est crucial pour la préparation
 * et l'exécution de commandes dans un environnement de shell.
 *
 * @parametres:
 * - lexer_list: t_lexer *lexer_list, pointeur vers le début de la liste de
 * lexèmes.
 *
 * @fonctionnement:
 * Parcourt la liste de lexèmes et incrémente un compteur chaque fois qu'un
 * token CMD est rencontré, indiquant une commande distincte. Cette méthode
 * permet de comprendre la structure et la quantité des commandes à exécuter.
 *
 * Pourquoi compter les commandes ?
 * - Organisation des Commandes : Comprendre le nombre de commandes permet de
 * structurer correctement l'exécution des commandes et de préparer les
 * structures de données nécessaires.
 * - Préparation de l'Exécution : Avoir un compte précis des commandes aide à
 * allouer les ressources nécessaires et à initialiser les structures de
 * contrôle adéquates pour l'exécution.
 * - Clarification de l'Entrée Utilisateur : Permet de décomposer l'entrée de
 * l'utilisateur en unités exécutables distinctes, facilitant l'analyse et la
 * gestion des commandes.
 *
 * @valeur_retour: 
 *   Renvoie le nombre total de commandes (int) dans la liste de lexèmes.
 *
 * @erreurs: 
 *   - Aucune erreur spécifique n'est générée par cette fonction. 
 *
 * @exemple_utilisation:
 *   t_lexer *lexer_list = create_lexer_list("cmd1 && cmd2");
 *   int total_cmds = count_cmd_in_lexer_linked_list(lexer_list);
 *
 * @dependances: 
 *   Aucune dépendance externe.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser le compteur 'index' à 0
 *     |
 *     v
 *   Parcourir la liste de lexèmes
 *     |
 *     v
 *   Token actuel est-il CMD ?
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   Incrementer  Passer au lexème suivant
 *   'index'     |
 *   |           v
 *   v         Fin de la liste ?
 *   Retourner   /       \
 *   à la       NON      OUI
 *   vérification |
 *   du token     v
 *                Fin de la boucle
 *                  |
 *                  v
 *                Retourner 'index'
 *                  |
 *                  v
 *                 Fin
 */
int	count_cmd_in_lexer_linked_list(t_lexer *lexer_list)
{
	int	cmd_count;

	cmd_count = ZERO_INIT;
	while (lexer_list)
	{
		if (is_current_lexer_token_cmd(lexer_list))
			cmd_count++;
		lexer_list = lexer_list->next;
	}
	return (cmd_count);
}

bool	is_pipe_or_end_of_lexer_linked_list(t_lexer *element)
{
    return (element->token == PIPE || element->next == NULL);
}

/**
 * @nom: count_pipe_or_end_in_lexer_linked_list
 *
 * @description:
 * Compte le nombre de nœuds (commandes ou pipelines) dans une liste de
 * lexèmes. Cette fonction est cruciale pour déterminer la structure et le
 * nombre de commandes à exécuter dans un environnement de shell.
 *
 * @parametres:
 * - lexer_list: t_lexer *lexer_list, pointeur vers le début de la liste de
 * lexèmes.
 *
 * @fonctionnement:
 * Parcourt la liste de lexèmes et incrémente un compteur chaque fois qu'un
 * token PIPE est rencontré ou à la fin de la liste. Chaque incrémentation
 * représente une commande ou un groupe de commandes (pipeline).
 *
 * Pourquoi compter les nœuds ?
 * - Préparation de l'Exécution : Permet de déterminer combien de structures
 * de commande doivent être préparées pour l'exécution des instructions du
 * shell.
 * - Gestion des Pipelines : Les pipelines sont des séquences de commandes
 * liées par des pipes (|). Comprendre leur nombre aide à organiser
 * l'exécution et la redirection des données entre les commandes.
 * - Structuration des Données : Offre une base pour structurer les données de
 * commande de manière à refléter la séquence et la logique de l'entrée de
 * l'utilisateur.
 *
 * @valeur_retour: 
 * Renvoie le nombre total de nœuds (int) dans la liste de lexèmes.
 *
 * @erreurs: 
 *   - Aucune erreur spécifique n'est générée par cette fonction. 
 *
 * @exemple_utilisation:
 *   t_lexer *lexer_list = create_lexer_list("cmd1 | cmd2");
 *   int total_nodes = count_pipe_or_end_in_lexer_linked_list(lexer_list);
 *
 * @dependances: 
 *   Aucune dépendance externe.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser le compteur 'index' à 0
 *     |
 *     v
 *   Parcourir la liste de lexèmes
 *     |
 *     v
 *   Token actuel est-il PIPE ou fin de liste ?
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   Incrementer  Passer au lexème suivant
 *   'index'     |
 *   |           v
 *   v         Fin de la liste ?
 *   Retourner   /       \
 *   à la       NON      OUI
 *   vérification |
 *   du token     v
 *                Fin de la boucle
 *                  |
 *                  v
 *                Retourner 'index'
 *                  |
 *                  v
 *                 Fin
 */
int	count_pipe_or_end_in_lexer_linked_list(t_lexer *lexer_list)
{
	int	index;

	index = ZERO_INIT;
	while (lexer_list)
	{
		if (is_pipe_or_end_of_lexer_linked_list(lexer_list))
			index++;
		lexer_list = lexer_list->next;
	}
	return (index);
}
