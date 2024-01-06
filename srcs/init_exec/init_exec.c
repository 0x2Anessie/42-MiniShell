#include "../../include/minishell.h"

/**
 * @nom: insert_command_at_end_of_linked_list
 *
 * @description:
 * Ajoute un nouveau nœud à la fin d'une liste de nœuds. Cette fonction est
 * essentielle pour construire et étendre des listes de nœuds, telles que des
 * listes de commandes ou de structures de données dans un shell.
 *
 * @parametres:
 * - node_lst: t_node *node_lst, pointeur vers le premier nœud de la liste
 * existante.
 * - new: t_node *new, pointeur vers le nouveau nœud à ajouter à la fin de la
 * liste.
 *
 * @fonctionnement:
 * Vérifie si la liste existante est vide (NULL). Si c'est le cas, retourne
 * directement le nouveau nœud comme début de la liste. Sinon, parcourt la
 * liste jusqu'au dernier nœud et ajoute le nouveau nœud à la fin. Cette
 * méthode assure que la liste de nœuds reste connectée et ordonnée
 * correctement après chaque ajout.
 *
 * Pourquoi cette fonction est-elle importante ?
 * - Gestion Dynamique des Listes : Permet de construire et d'étendre des
 * listes de nœuds dynamiquement, ce qui est crucial pour gérer des séquences
 * ou des collections de données qui évoluent au cours de l'exécution d'un
 * programme.
 * - Maintien de l'Ordre : Assure que l'ordre des nœuds dans la liste est
 * préservé, ce qui est important pour des opérations telles que l'exécution
 * séquentielle de commandes dans un shell.
 * - Flexibilité dans la Manipulation de Données : Offre une méthode simple et
 * efficace pour manipuler des listes de structures de données, en permettant
 * l'ajout d'éléments sans perturber la structure existante de la liste.
 *
 * @valeur_retour: 
 * Renvoie un pointeur vers le début de la liste de nœuds après l'ajout du
 * nouveau nœud.
 *
 * @erreurs: 
 * - Aucune erreur spécifique n'est générée par cette fonction. 
 *
 * @exemple_utilisation:
 *   t_node *head = create_node_list();
 *   t_node *new_node = create_new_node();
 *   head = insert_command_at_end_of_linked_list(head, new_node);
 *
 * @dependances: 
 *   Aucune dépendance externe.
 * 
  * @graphe_de_flux:
 *  Début
 *   |
 *   v
 *   - Vérifier si la liste existante (node_lst) est NULL
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Retourner le nouveau nœud (new)
 *   - Parcourir la liste jusqu'au dernier nœud
 *   - Nœud actuel (tmp) a-t-il un suivant (next) ?
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Ajouter le nouveau nœud (new) à la fin de la liste
 *   - Passer au nœud suivant (tmp = tmp->next)
 *   - Retour à la vérification du nœud actuel
 *   |
 *   v
 *   - Retourner le début de la liste (node_lst)
 *   |
 *   v
 *  Fin
 */
t_node	*insert_command_at_end_of_linked_list(t_node *node_lst, t_node *new)
{
	t_node	*tmp;

	tmp = node_lst;
	if (!tmp)
		return (new);
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (node_lst);
}

/**
 * @nom: build_cmd_linked_list
 *
 * @description:
 * Construit une liste de nœuds représentant chaque commande ou groupe de
 * commandes dans une séquence de commandes shell. Cette fonction est cruciale
 * pour organiser les commandes et les pipelines en une structure de données
 * qui facilite leur exécution et gestion.
 *
 * @parametres:
 * - node: t_node *node, pointeur vers le nœud actuel dans la liste.
 * - lexer_lst: t_lexer *lexer_lst, pointeur vers la liste de lexèmes
 * représentant les commandes.
 * - utils: t_exec *utils, pointeur vers la structure contenant les
 * informations d'exécution.
 *
 * @fonctionnement:
 * Parcourt la liste de lexèmes et pour chaque commande ou groupe de commandes,
 * effectue les opérations suivantes :
 * - Alloue un nouveau nœud pour la commande ou le pipeline en utilisant
 * `ft_malloc_with_tracking`.
 * - Configure les redirections d'entrée et de sortie pour chaque nœud avec
 * `setup_input_redirection` et `setup_output_redirection`.
 * - Vérifie la présence de la commande dans le lexème en utilisant `is_command_present`.
 * - Ajoute le nœud configuré à la fin de la liste existante en utilisant
 * `insert_command_at_end_of_linked_list`.
 * Répète ce processus jusqu'à avoir parcouru tous les lexèmes ou atteint un
 * token PIPE.
 *
 * Pourquoi cette fonction est-elle importante ?
 * - Structuration des Commandes : Organiser les commandes en nœuds distincts
 * facilite leur traitement séquentiel ou parallèle, ce qui est crucial dans
 * les environnements de shell où les pipelines et les redirections sont
 * fréquents.
 * - Gestion des Redirections : Configurer les redirections d'entrée et de
 * sortie pour chaque commande individuellement assure que les flux de données
 * sont correctement dirigés selon les besoins spécifiques de chaque commande.
 * - Flexibilité et Clarté : Avoir une structure de données claire pour chaque
 * commande offre une plus grande flexibilité dans la manière dont les
 * commandes sont exécutées et rend le processus global plus transparent et
 * facile à suivre.
 * - Préparation à l'Exécution : Cette étape de préparation est essentielle
 * pour assurer que toutes les commandes sont prêtes à être exécutées avec
 * leurs contextes et configurations appropriés, ce qui est fondamental pour
 * le bon fonctionnement du shell.
 * 
 * @valeur_retour: 
 *   Aucune (void). Construit et modifie la liste de nœuds en place.
 *
 * @erreurs: 
 * - Si l'allocation de mémoire pour un nouveau nœud échoue, la fonction se
 * termine prématurément.
 *
 * @exemple_utilisation:
 * t_node *node = NULL;
 * t_lexer *lexer_lst = create_lexer_list();
 * t_exec utils;
 * build_cmd_linked_list(node, lexer_lst, &utils);
 *
 * @dependances: 
 * - ft_malloc_with_tracking pour l'allocation mémoire avec suivi.
 * - setup_input_redirection et setup_output_redirection pour configurer les
 * redirections.
 * - is_command_present pour vérifier la présence de commandes.
 * - insert_command_at_end_of_linked_list pour ajouter un nœud à la liste.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser 'index' à 0
 *     |
 *     v
 *   Entrer dans la boucle pour chaque nœud (index < nb_node)
 *     |
 *     v
 *   Allouer un nouveau nœud (ft_malloc_with_tracking)
 *     |
 *     v
 *   Allocation réussie ?
 *   /       \
 * NON       OUI
 *   |         |
 *   v         v
 * Retourner  Configurer les redirections (setup_input_redirection,
 * sans rien  setup_output_redirection)
 * faire       |
 *             v
 *           Vérifier si le lexème contient une commande (is_command_present)
 *             |
 *             v
 *           Ajouter le nœud à la liste (insert_command_at_end_of_linked_list)
 *             |
 *             v
 *           Avancer jusqu'au prochain PIPE ou fin de liste
 *             |
 *             v
 *           Incrementer 'index'
 *             |
 *             v
 *   Fin de la boucle
 *     |
 *     v
 *   Restaurer le lexème de départ
 *     |
 *     v
 *   Fin
 */
void	build_cmd_linked_list(t_node *node, t_data *data, t_exec *utils)
{
	int	index;

	index = ZERO_INIT;
	while (index < utils->nb_node)
	{
		node = ft_malloc_with_tracking(data, sizeof(t_node));
		if (!node)
			return ;
		node->index = index;
		setup_input_redirection(node, data->lexer_list, data);
		setup_output_redirection(node, data->lexer_list);
		node->next = NULL;
		node->is_command_present = is_token_type_cmd(data->lexer_list);
		utils->node = insert_command_at_end_of_linked_list(utils->node, node);
		while (data->lexer_list && data->lexer_list->token != PIPE)
			data->lexer_list = data->lexer_list->next;
		if (data->lexer_list)
			data->lexer_list = data->lexer_list->next;
		index++;
	}
	data->lexer_list = utils->head_lexer_lst;
}

/**
 * @nom: ft_init_exec
 *
 * @description:
 * Initialise l'environnement d'exécution pour le traitement des commandes
 * shell, en construisant et en organisant les structures de données
 * nécessaires. Cette fonction est essentielle pour préparer l'environnement
 * avant l'exécution des commandes et gérer correctement les variables
 * d'environnement et les redirections.
 *
 * @parametres:
 * - data: t_data *data, structure contenant les listes de lexèmes et
 * l'environnement.
 *
 * @fonctionnement:
 * Initialise les structures globales et locales nécessaires à l'exécution des
 * commandes.
 * - Compte le nombre de commandes et de nœuds (pipelines) dans les lexèmes.
 * - Prépare la liste d'exportation des variables d'environnement.
 * - Construit la liste des nœuds de commande avec `build_cmd_linked_list`,
 * qui crée des nœuds pour chaque commande et configure les redirections
 * d'entrée et de sortie.
 * - Gère les signaux pour le traitement du here-doc avec `handle_sig`.
 * - Prépare les citations dans les variables d'exportation avec
 * `export_quotes`.
 *
 * Pourquoi initialiser un environnement d'exécution ?
 * - Préparation de l'environnement : Assure que toutes les structures
 * nécessaires sont prêtes avant l'exécution des commandes.
 * - Gestion des commandes et pipelines : Organise les commandes et les
 * redirections dans une structure de données adaptée pour une exécution
 * efficace.
 * - Gestion des variables d'environnement : Prépare et organise les variables
 * d'environnement pour une utilisation et une exportation correctes.
 *
 * @valeur_retour: 
 * Aucune (void). Modifie les structures globales et locales pour
 * l'exécution des commandes.
 *
 * @erreurs: 
 *   - Aucune erreur spécifique n'est générée par cette fonction. 
 *
 * @exemple_utilisation:
 *   t_data *data;
 *   // Initialiser data avec les listes de lexèmes et l'environnement...
 *   ft_init_exec(&data);
 *
 * @dependances: 
 * - build_cmd_linked_list pour construire la liste des nœuds de commande.
 * - ft_buil_sorted_linked_list_env_var pour construire la liste
 * d'exportation des variables d'environnement.
 * - export_quotes pour préparer les citations dans les variables
 * d'exportation.
 * - handle_sig pour la gestion des signaux pendant le here-doc.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser les structures et variables globales de gestion
 *     |
 *     v
 *   Compter le nombre de commandes et de nœuds (total_number_of_cmd_find_in_linked_list, nb_node)
 *     |
 *     v
 *   La liste d'exportation
 * des variables d'environnement
 * est-elle déjà initialisée ?
 *  /          \
 * OUI         NON
 *  |           |
 *  v        v
 * Passer   Initialiser la liste d'exportation 
 * à l'étape  |
 * suivante  v
 *          Préparer les citations dans les variables (export_quotes)
 *             |
 *             v
 *          Initialiser un pointeur de nœud (node) à NULL
 *             |
 *             v
 *          Entrer dans la boucle pour créer chaque nœud de commande
 *            (build_cmd_linked_list)
 *             |
 *             v
 *          Configurer les redirections d'entrée et de sortie pour chaque
 *           nœud
 *             |
 *             v
 *          Gérer les signaux pour le here-doc (handle_sig)
 *             |
 *             v
 *          Ajouter le nœud créé à la liste des nœuds de commande
 *             |
 *             v
 *          Passer au lexème suivant jusqu'à la fin de la liste de lexèmes
 *             |
 *             v
 *          Réinitialiser l'indicateur de here-doc et gérer à nouveau les
 *           signaux
 *             |
 *             v
 *          Mettre à jour la tête de la liste des nœuds de commande
 *             |
 *             v
 *          Fin
 */
void	ft_init_exec(t_data *data)
{
	t_node	*node;

	data->utils->node = NULL;
	data->utils->is_here_doc = 1;
	data->utils->can_run = 1;
	data->utils->hd = NULL;
	data->utils->total_number_of_cmd_find_in_linked_list = \
	count_cmd_in_lexer_linked_list(data->lexer_list);
	data->utils->nb_node = count_pipe_or_end_in_lexer_linked_list(data->lexer_list);
	data->utils->head_lexer_lst = data->lexer_list;
	data->utils->env = data->env;
	if (!(data->utils->head_of_linked_list_env_var))
	{
		data->utils->head_of_linked_list_env_var = \
		ft_buil_sorted_linked_list_env_var(data, data->utils->head_of_linked_list_env_var);
		export_quotes(data->utils->head_of_linked_list_env_var, data);
	}
	node = NULL;
	build_cmd_linked_list(node, data, data->utils);
	data->utils->is_here_doc = ZERO_INIT;
	handle_sig(data);
	data->utils->head_node_lst = data->utils->node;
}
