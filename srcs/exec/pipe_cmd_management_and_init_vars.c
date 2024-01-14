#include "../../include/minishell.h"

bool	is_current_token_cmd_arg(t_lexer *lexer_lst)
{
	return (lexer_lst && lexer_lst->token == ARG);
}

/**
 * @nom: reaches_next_cmd_preceded_by_pipe
 * @brief: Avance dans la liste de lexèmes jusqu'à la prochaine commande.
 *
 * @description:
 * Parcourt une liste de lexèmes (t_lexer) et avance jusqu'à ce qu'elle trouve
 * un lexème représentant un pipe (PIPE), indiquant la fin d'une commande et le
 * début de la suivante. Cette fonction est utilisée pour naviguer à travers
 * les différentes commandes dans un environnement de shell.
 *
 * @param lexer_list: t_lexer *lexer_list, pointeur vers le début de la liste
 * de lexèmes à parcourir.
 *
 * @pourquoi:
 * - Navigation entre les Commandes : Permet de passer facilement d'une
 * commande à l'autre dans un pipeline, ce qui est crucial pour l'exécution
 * séquentielle des commandes dans un shell.
 * - Simplification du Traitement des Commandes : Offre une méthode simple
 * pour ignorer les lexèmes jusqu'à la prochaine commande, facilitant ainsi le
 * traitement et l'analyse des commandes individuelles.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur sur le lexème suivant après le PIPE, ce qui correspond
 * au début de la prochaine commande, ou NULL si la fin de la liste est
 * atteinte.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Aucun effet de bord majeur n'est attendu si la liste de lexèmes est bien
 * formée.
 *
 * @exemples_utilisation:
 * t_lexer *lexer_list = create_lexer_list("cmd1 arg1 | cmd2 arg2");
 * lexer_list = reaches_next_cmd_preceded_by_pipe(lexer_list); // Avance à 'cmd2'
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Parcourir 'lexer_list'
 *     |
 *     v
 *   'lexer_list' est-il NULL ou token == PIPE ?
 *  /       \
 * NON      OUI
 *  |         |
 *  v         v
 * Passer au  'lexer_list' est-il toujours non NULL ?
 * lexème suivant  /       \
 *               NON      OUI
 *                |         |
 *                v         v
 *              Retourner  Passer au lexème suivant
 *              NULL       (après le PIPE)
 *                           |
 *                           v
 *                        Retourner 'lexer_list'
 *                           |
 *                           v
 *                         Fin
 */
t_lexer	*reaches_next_cmd_preceded_by_pipe(t_lexer *lexer_list)
{
	while (is_current_token_not_pipe(lexer_list))
		lexer_list = lexer_list->next;
	if (is_current_token_pipe(lexer_list))
		lexer_list = lexer_list->next;
	return (lexer_list);
}

/**
 * @nom: count_args_until_pipe_for_cmd_array
 *
 * @description: Compte le nombre d'arguments dans une liste de lexèmes
 * jusqu'à un token de type PIPE.
 *
 * @parametres:
 * - lexer_list: t_lexer *lexer_list, pointeur vers le début de la liste de
 * lexèmes.
 *
 * @fonctionnement:
 * La fonction initialise un index à 1. Elle parcourt ensuite la liste de
 * lexèmes en continuant tant que le token actuel n'est pas de type PIPE. Pour
 * chaque token de type ARG rencontré, l'index est incrémenté. Cette méthode
 * permet de compter le nombre d'arguments dans une commande jusqu'au prochain
 * PIPE, ce qui est crucial dans l'interprétation des commandes dans un shell,
 * où les arguments sont séparés par des espaces et les commandes par des
 * pipes.
 *
 * Pourquoi compter les arguments ?
 * - Analyse syntaxique : Permet de déterminer le nombre d'arguments d'une
 * commande, ce qui est essentiel pour sa bonne exécution.
 * - Préparation à la segmentation des commandes : Dans un shell, les commandes
 * peuvent être chaînées avec des pipes. Compter les arguments jusqu'au
 * prochain PIPE permet de segmenter correctement les commandes pour leur
 * traitement ultérieur.
 *
 * @valeur_retour: 
 * Renvoie le nombre total d'arguments trouvés dans la liste jusqu'au token
 * PIPE.
 *
 * @erreurs: 
 * - Aucune erreur particulière n'est retournée par cette fonction.
 *
 * @exemple_utilisation:
 * t_lexer *lexer_list = create_lexer_list("cmd arg1 arg2 | cmd2");
 * int nb_args = count_args_until_pipe_for_cmd_array(lexer_list);
 *
 * @dependances: 
 * - Aucune dépendance externe autre que la structure t_lexer et les
 * définitions des tokens.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser 'index' à 1 (comptant la commande elle-même)
 *     |
 *     v
 *   Parcourir 'lexer_list'
 *     |
 *     v
 *   'lexer_list' est-il NULL ou token == PIPE ?
 *  /       \
 * OUI      NON
 *  |         |
 *  v         v
 * Retourner  'lexer_list->token' est-il ARG ?
 * 'index'   /       \
 *          OUI      NON
 *           |         |
 *           v         v
 *        Incrémenter  Passer au lexème suivant
 *        'index'     
 *           |
 *           v
 *        Fin de la boucle
 *           |
 *           v
 *        Fin
 */
int	count_args_until_pipe_for_cmd_array(t_lexer *lexer_list)
{
	int	index;

	index = 1;
	while (is_current_token_not_pipe(lexer_list))
	{
		if (is_current_token_cmd_arg(lexer_list))
			index++;
		lexer_list = lexer_list->next;
	}
	return (index);
}

bool	is_index_less_than_num_nodes(int index, int num_nodes)
{
	return (index < num_nodes);
}

/**
 * @nom: initialize_pid_array_to_zero
 * @brief: Initialise un tableau de Process ID (PID) à zéro.
 *
 * @description:
 * La fonction parcourt un tableau de type pid_t (généralement utilisé pour
 * stocker des identifiants de processus) et initialise chaque élément à zéro.
 * Cette initialisation est essentielle pour assurer que le tableau ne contient
 * pas de valeurs indésirables ou non définies avant son utilisation.
 *
 * @param pid_array: pid_t *pid_array, pointeur vers le tableau de PID à
 * initialiser.
 * @param num_nodes: int num_nodes, nombre d'éléments dans le tableau de PID.
 *
 * @pourquoi:
 * - Initialisation Claire des PID : Assure que tous les PIDs dans le tableau
 * sont définis à une valeur connue (zéro), évitant les confusions ou erreurs
 * liées à des valeurs non initialisées.
 * - Préparation à la Gestion des Processus : Prépare le tableau pour un usage
 * fiable dans la gestion des processus, où l'identification correcte et unique
 * des processus est cruciale.
 *
 * @valeur_de_retour: 
 * Aucune (void). La fonction remplit son objectif d'initialisation sans
 * retourner de valeur.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Aucun effet de bord ni erreur attendu si 'pid_array' est valide et
 * 'num_nodes' est correctement défini.
 *
 * @exemples_utilisation:
 * pid_t pid_array[5];
 * int num_nodes = 5;
 * initialize_pid_array_to_zero(pid_array, num_nodes);
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialisation de index à ZERO_INIT
 *     |
 *     v
 *   Vérification si pid_array est NULL
 *  /        \
 * NON       OUI
 *  |         \
 *  |          \
 *  v           v
 * Parcourir   Fin de la
 * pid_array   fonction
 *     |
 *     v
 *   Index < num_nodes ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Initialiser  Fin de la
 * pid_array[index] à 0   fonction
 * et incrémenter index
 *     |
 *     v
 *   Fin
 */
void	initialize_pid_array_to_zero(pid_t *pid_array, int num_nodes)
{
	int	index;

	index = ZERO_INIT;
	if (!pid_array)
		return ;
	while (is_index_less_than_num_nodes(index, num_nodes))
	{
		pid_array[index] = 0;
		index++;
	}
}

/**
 * @nom: init_fd_and_status_vars
 * @brief: Initialise des variables utilisées dans la gestion des processus et
 * des descripteurs de fichier.
 *
 * @description:
 * Cette fonction initialise plusieurs variables importantes utilisées dans le
 * cadre de la gestion des processus et des descripteurs de fichier dans un
 * shell ou un système similaire. Elle met à zéro ou initialise à des valeurs
 * par défaut les variables passées en paramètre.
 *
 * @param fd: int *fd, tableau de descripteurs de fichier à initialiser.
 * @param y: int *y, tableau d'entiers utilisé pour le suivi des indices ou des
 * états.
 * @param wstatus: int *wstatus, pointeur vers un entier pour stocker le statut
 * de sortie d'un processus.
 *
 * @pourquoi:
 * - Initialisation Cohérente : Assure que les variables sont dans un état
 * connu et cohérent avant leur utilisation, évitant des comportements imprévus
 * dus à des valeurs non initialisées.
 * - Préparation pour le Traitement des Processus : Met en place les variables
 * nécessaires pour la gestion des processus et des descripteurs de fichier,
 * facilitant les opérations ultérieures comme la création de pipes ou la
 * gestion de la sortie des processus.
 *
 * @valeur_de_retour:
 * Aucune (void). La fonction sert uniquement à initialiser des variables.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Aucun effet de bord ni erreur n'est associé à cette fonction.
 *
 * @exemples_utilisation:
 * int fd[2];
 * int y[2];
 * int wstatus;
 * init_fd_and_status_vars(fd, y, &wstatus);
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser 'wstatus' à ZERO_INIT
 *     |
 *     v
 *   Initialiser 'y[0]' à ZERO_INIT et 'y[1]' à -1
 *     |
 *     v
 *   Initialiser 'fd[0]' et 'fd[1]' à ZERO_INIT
 *     |
 *     v
 *   Fin
 */
void	init_fd_and_status_vars(int *fd, int *y, int *wstatus)
{
	*wstatus = ZERO_INIT;
	y[0] = ZERO_INIT;
	y[1] = -1;
	fd[0] = ZERO_INIT;
	fd[1] = ZERO_INIT;
}
