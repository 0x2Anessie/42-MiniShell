#include "../../include/minishell.h"

/**
 * @fonction: ft_exec_single_built_in
 * @breve_description: Exécute une commande intégrée unique à partir d'une
 * liste de lexèmes.
 *
 * @param lexer_lst: Pointeur sur t_lexer, la liste des lexèmes à analyser.
 * @param fd: Pointeur sur un entier représentant un descripteur de fichier.
 *
 * @description_detaillee:
 * La fonction 'ft_exec_single_built_in' a pour rôle d'exécuter une commande
 * intégrée spécifique, identifiée dans la liste des lexèmes ('lexer_lst').
 * Elle utilise 'is_exact_command_match' pour comparer le mot actuel avec une
 * série de commandes intégrées connues (comme 'export', 'pwd', 'echo', etc.),
 * et appelle la fonction appropriée pour exécuter la commande.
 *
 * Pourquoi exécuter des commandes intégrées individuellement ?
 * - Modularité : Chaque commande intégrée peut nécessiter une logique
 * différente. En les traitant individuellement, on maintient une structure
 * claire et modulaire.
 * - Séparation des responsabilités : Cette approche permet de déléguer la
 * responsabilité de l'exécution à des fonctions spécialisées, améliorant ainsi
 * la lisibilité et la maintenance du code.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'lexer_lst' est NULL, les fonctions spécifiques aux commandes intégrées
 * ne seront pas appelées.
 * - La fermeture du descripteur de fichier
 * (si 'data->utils->node->output_fd > 0') doit être gérée avec soin pour
 * éviter des fuites de ressources.
 *
 * @exemples_utilisation:
 * t_lexer lexer;
 * int fd = 1;
 * lexer.word = "export";
 * ft_exec_single_built_in(&lexer, &fd);
 *
 * @dependances:
 * - is_exact_command_match() : Pour comparer les commandes.
 * - export_things(), get_pwd(), init_echo(), etc. : Fonctions spécifiques aux
 * commandes intégrées.
 * - data->utils->node : Pour gérer les descripteurs de fichiers.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la commande dans lexer_lst 
 * (export, pwd, echo, cd, env, unset, exit)
 *  /        \						    \
 * CMD      CMD							CMD
 * EXPORT   PWD							EXIT
 *  |         |							 |
 *  |         |							 |
 *  v         v 						 v
 * Exécuter  Exécuter				  Exécuter
 * export    pwd						exit
 *            |
 *            |
 *            v
 *        Fermer le descripteur de fichier si nécessaire
 *            |
 *            v
 *        Réinitialiser lexer_lst à la tête de la liste
 */
void	ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data)
{
	if (is_cmd_match(lexer_lst, CMD_EXPORT_VARS, strlen(CMD_EXPORT_VARS)))
		export_things(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_PRINT_DIRCT, strlen(CMD_PRINT_DIRCT)))
		get_pwd(lexer_lst->cmd_segment, data);
	else if (is_cmd_match(lexer_lst, CMD_ECHO, strlen(CMD_ECHO)))
		init_echo(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_CHANG_DIRCT, strlen(CMD_CHANG_DIRCT)))
		get_cd(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_ENV_VARS, strlen(CMD_ENV_VARS)))
		get_unset(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_UNSET_VARS, strlen(CMD_UNSET_VARS)))
		get_unset(lexer_lst, data);
	else if (is_cmd_match(lexer_lst, CMD_EXIT_SHELL, strlen(CMD_EXIT_SHELL)))
		ft_exit(lexer_lst, fd, data);
	if (data->utils->node->output_fd > 0)
		close(data->utils->node->output_fd);
	lexer_lst = data->utils->head_lexer_lst;
}

/**
 * @fonction: close_fds_if_needed
 * @breve_description: Ferme les descripteurs de fichiers spécifiés si
 * nécessaire.
 *
 * @param fd: Pointeur sur un tableau d'entiers contenant les descripteurs de
 * fichiers à évaluer.
 * @param previous_fd: Entier représentant un descripteur de fichier
 * supplémentaire à évaluer.
 *
 * @description_detaillee:
 * La fonction 'close_fds_if_needed' examine et ferme les descripteurs de
 * fichiers spécifiés si leurs valeurs indiquent qu'ils sont ouverts
 * (c'est-à-dire, supérieurs à 0). Cette fonction est utilisée pour gérer
 * correctement les ressources système en fermant les descripteurs de fichiers
 * qui ne sont plus nécessaires.
 *
 * Pourquoi la fermeture conditionnelle des descripteurs de fichiers est-elle
 * importante ?
 * - Prévention des fuites de ressources : La fermeture appropriée des
 * descripteurs de fichiers évite les fuites de ressources, ce qui est crucial
 * pour la stabilité et les performances du système.
 * - Bonne gestion des ressources : Assure que les descripteurs de fichiers
 * sont libérés quand ils ne sont plus utilisés, permettant ainsi une
 * utilisation plus efficace des ressources du système.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'fd' est NULL ou si 'fd[1]' n'est pas un descripteur de fichier valide,
 * la fermeture peut échouer ou avoir un comportement indéfini.
 * - La fermeture de 'previous_fd' peut échouer si ce n'est pas un descripteur
 * de fichier valide.
 *
 * @exemples_utilisation:
 * int fds[2] = {1, 2};
 * int prev_fd = 3;
 * close_fds_if_needed(fds, prev_fd); // Ferme fds[1] et prev_fd si ouverts
 *
 * @dependances: Fonction 'close' de la bibliothèque standard pour la
 * fermeture des descripteurs.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   fd[1] > 0 ?
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * Fermer     previous_fd > 0 ?
 * fd[1]     /        \
 *           VRAI     FAUX
 *            |        |
 *            |        |
 *            v        v
 *          Fermer    Terminer
 *         previous_fd
 */
void	close_fds_if_needed(int *fd, int previous_fd)
{
	if (is_fd_valid_for_close(fd[1]))
		close(fd[1]);
	if (is_fd_valid_for_close(previous_fd))
		close(previous_fd);
}

/**
 * @fonction: manage_exec_linked_cmd_sequence
 * @breve_description: Démarre l'exécution des commandes dans une liste de
 * lexèmes.
 *
 * @param fd: Pointeur sur un tableau d'entiers pour les descripteurs de
 * fichiers (pipes).
 * @param pid: Pointeur sur un tableau de pid_t pour stocker les PID des
 * processus enfants.
 * @param lex_lst: Pointeur sur une structure t_lexer représentant la liste des
 * lexèmes.
 * @param y: Pointeur sur un tableau d'entiers utilisé pour la logique de
 * contrôle.
 *
 * @description_detaillee:
 * La fonction 'manage_exec_linked_cmd_sequence' orchestre l'exécution des
 * commandes en parcourant une liste de lexèmes. Elle initialise 'lex_lst'
 * avec la tête de la liste de lexèmes globale, puis entre dans une boucle
 * contrôlée par 'should_continue_execution'. Dans cette boucle, elle trouve la
 * prochaine commande, gère les pipes, exécute des commandes intégrées ou crée
 * des processus enfants pour les autres commandes, et gère les descripteurs de
 * fichiers ouverts.
 *
 * Pourquoi cette orchestration est-elle cruciale ?
 * - Coordination des commandes : Assure que chaque commande est exécutée dans
 * le bon ordre et avec les bons paramètres, en particulier dans un
 * environnement avec pipes et redirections.
 * - Gestion des ressources : La création et la fermeture correctes des pipes
 * et des descripteurs de fichiers préviennent les fuites de ressources.
 *
 * @valeur_de_retour:
 * Retourne 1 (succès) si toutes les commandes sont exécutées correctement, 0
 * (échec) en cas d'erreur.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si une commande intégrée échoue ou si 'pipe(fd)' renvoie une erreur, la
 * fonction retourne 0.
 * - La modification des valeurs de 'lex_lst' et 'data->utils->node' a un effet
 * sur l'état global du programme.
 *
 * @exemples_utilisation:
 * int fd[2], y[2] = {0, 0};
 * pid_t pid[10];
 * t_lexer *lex_lst;
 * int result = manage_exec_linked_cmd_sequence(fd, pid, lex_lst, y);
 * -------> Résultat attendu : 1 ou 0
 *
 * @dependances:
 * - should_continue_execution, find_next_command_in_lexer,
 * check_redirection_validity_in_node, etc. :
 * Fonctions auxiliaires pour la logique d'exécution.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser lex_lst avec la tête de la liste de lexèmes globale
 * (data->utils->head_lexer_lst)
 *   |
 *   v
 * Entrer dans la boucle tant que should_continue_execution retourne vrai
 *   |
 *   v
 * Trouver la prochaine commande avec find_next_command_in_lexer
 *   |
 *   v
 * Initialiser data->utils->previous_fd avec fd[0]
 *   |
 *   v
 * data->utils->total_number_of_cmd_find_in_linked_list >= 1 et pipe(fd) échoue ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner 0  Continuer l'exécution
 *   |
 *   v
 * check_redirection_validity_in_node de data->utils->node est vrai ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Continuer   Passer à la prochaine commande sans exécuter
 * l'exécution
 *   |
 *   v
 * (is_built_in_command(lex_lst))
 * et (data->utils->total_number_of_cmd_find_in_linked_list == 1) ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Exécuter   Créer un processus enfant avec create_and_execute_child_process
 * et ft_exec_single_built_in  stocker le PID dans pid[y[0]++]
 *   |
 *   v
 * Passer à la prochaine commande avec reaches_next_cmd_preceded_by_pipe
 *   |
 *   v
 * Fermer les descripteurs de fichiers ouverts avec close_fds_if_needed
 *   |
 *   v
 * Passer au nœud suivant dans data->utils->node
 *   |
 *   v
 * Retour à la condition de la boucle (should_continue_execution)
 *   |
 *   v
 * Fin de la boucle
 *   |
 *   v
 * Fermer les pipes restants avec closes_ends_of_pipe
 *   |
 *   v
 * Retourner 1
 * Fin
 */
int	manage_exec_linked_cmd_sequence(int *fd, pid_t *pid, t_data *data, int *y)
{
	data->lexer_list = data->utils->head_lexer_lst;
	while (should_continue_execution(data, y))
	{
		data->lexer_list = find_next_command_in_lexer(data->lexer_list);
		data->utils->previous_fd = fd[0];
		if (is_pipe_creation_failed(\
		fd, data->utils->total_number_of_cmd_find_in_linked_list))
			return (0);
		if (check_redirection_validity_in_node(data->utils->node))
		{
			if (is_single_builtin_command(data))
				ft_exec_single_built_in(data->lexer_list, fd, data);
			else
				pid[y[0]++] = create_and_execute_child_process(\
				data, fd, y[1], *(data->utils));
		}
		data->lexer_list = reaches_next_cmd_preceded_by_pipe(data->lexer_list);
		close_fds_if_needed(fd, data->utils->previous_fd);
		data->utils->node = data->utils->node->next;
	}
	closes_ends_of_pipe(fd);
	return (1);
}

/**
 * @fonction: wait_and_process_child_statuses
 * @breve_description: Attend la terminaison des processus enfants spécifiés.
 *
 * @param pid: Pointeur sur un tableau de pid_t contenant les PID des processus
 * enfants.
 * @param wstatus: Pointeur sur un entier pour stocker le statut de terminaison
 * des processus.
 * @param nb_node: Entier représentant le nombre de processus enfants à
 * attendre.
 *
 * @description_detaillee:
 * La fonction 'wait_and_process_child_statuses' parcourt un tableau de PID de
 * processus enfants et attend leur terminaison. Pour chaque PID valide
 * (supérieur à 0), elle appelle 'waitpid' pour attendre la fin du processus
 * correspondant. Si un processus se termine normalement, elle met à jour
 * 'globi' avec le code de sortie du processus.
 *
 * Pourquoi attendre les processus enfants ?
 * - Prévention des processus zombies : Attendre les processus enfants permet
 * de libérer les ressources associées et d'éviter les processus zombies qui
 * pourraient encombrer le système.
 * - Récupération des codes de sortie : Cela permet de récupérer les codes de
 * sortie des processus enfants pour une gestion appropriée des erreurs ou des
 * états de sortie.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'pid' est NULL, la fonction retourne immédiatement sans attendre aucun
 * processus.
 * - La fonction dépend de 'data->utils->heredoc_ctrl_c_uninterrupted' et
 * 'data->utils->total_number_of_cmd_find_in_linked_list' pour
 * déterminer si elle doit continuer à attendre les processus.
 *
 * @exemples_utilisation:
 * pid_t pids[10];
 * int wstatus;
 * int nb_node = 5;
 * wait_and_process_child_statuses(pids, &wstatus, nb_node);
 * -------> Attend la fin de 5 processus enfants
 *
 * @dependances:
 * - waitpid() : Fonction de la bibliothèque standard pour attendre un
 * processus enfant spécifique.
 * - WIFEXITED et WEXITSTATUS : Macros pour analyser le statut renvoyé par
 * waitpid.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Vérifier si 'pid' est NULL
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retour   Initialiser 'index' à ZERO_INIT
 *   |         |
 *   |         v
 *   |       Entrer dans la boucle tant que 'nb_node' > 0,
 *   |       'data->utils->heredoc_ctrl_c_uninterrupted' 
 *   |       et 'data->utils->total_number_of_cmd_find_in_linked_list'
 *   |       sont vrais
 *   |         |
 *   |         v
 *   |       pid[index] est-il supérieur à 0 ?
 *   |      /       \
 *   |    NON       OUI
 *   |     |         |
 *   |     |         |
 *   |     v         v
 *   |   Passer à  Appeler waitpid(pid[index], wstatus, 0)
 *   |   l'index    |
 *   |   suivant    |
 *   |              v
 *   |            Processus s'est-il terminé normalement (WIFEXITED(*wstatus)) ?
 *   |           /       \
 *   |         NON       OUI
 *   |          |         |
 *   |          |         |
 *   |          v         v
 *   |         Passer à  Mettre à jour 'globi' avec WEXITSTATUS(*wstatus)
 *   |         l'index    |
 *   |         suivant    |
 *   |                     |
 *   |                     v
 *   |                  Passer à l'index suivant
 *   |                     |
 *   |                     v
 *   |                  Décrémenter 'nb_node'
 *   |                     |
 *   |                     v
 *   |                  Retour à la condition de la boucle
 *   |
 *   v
 * Fin
 */
void	wait_and_process_child_statuses(\
pid_t *pid, int *wstatus, int nb_node, t_data *data)
{
	int	index;

	index = ZERO_INIT;
	if (is_pid_array_null(pid))
		return ;
	while (should_continue_waiting_for_child_processes(nb_node, data))
	{
		if (is_process_pid_valid_for_wait(*pid))
		{
			waitpid(pid[index], wstatus, 0);
			if (data->utils->heredoc_nbr == 1)
			{
				g_globi = 0;
				data->utils->heredoc_nbr = 0;
			}
			else
				g_globi = 127;
		}
		index++;
		nb_node--;
	}
}

/**
 * @fonction: manage_execution_resources
 * @breve_description: Prépare et exécute les commandes à partir des données
 * fournies.
 *
 * @param data: Pointeur sur t_data contenant les données nécessaires à
 * l'exécution.
 *
 * @description_detaillee:
 * La fonction 'manage_execution_resources' coordonne l'exécution des commandes
 * en initialisant les variables nécessaires, en allouant de la mémoire pour
 * les PID des processus enfants, en gérant les signaux de processus, en
 * démarrant l'exécution des commandes, et en attendant la fin des processus
 * enfants. Elle gère également l'ouverture et la fermeture des descripteurs de
 * fichiers.
 *
 * Pourquoi cette préparation est-elle essentielle ?
 * - Cohérence de l'exécution : Assure que toutes les variables et ressources
 * nécessaires sont correctement initialisées avant l'exécution des commandes.
 * - Gestion des ressources : La gestion de la mémoire et des signaux est
 * cruciale pour le bon déroulement et la stabilité de l'exécution.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si l'allocation de 'pid' échoue, la fonction retourne sans exécuter les
 * commandes.
 * - Les appels à 'manage_exec_linked_cmd_sequence' et
 * 'wait_and_process_child_statuses' peuvent influencer l'état global et les
 * ressources du système.
 *
 * @exemples_utilisation:
 * t_data *data;
 * manage_execution_resources(&data);
 * -------> Prépare et exécute les commandes en fonction des données fournies
 *           dans 'data'
 *
 * @dependances:
 * - manage_exec_linked_cmd_sequence, wait_and_process_child_statuses,
 * close_file_descriptors_in_linked_list, etc. : Fonctions utilisées pour
 * l'exécution et la gestion des processus.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser les variables (fd, y, wstatus) avec 'init_fd_and_status_vars'
 *   |
 *   v
 * Allouer de la mémoire pour 'pid' avec 'ft_malloc_with_tracking'
 *   |
 *   v
 * pid est-il NULL ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retour   Initialiser 'pid' avec 'initialize_pid_array_to_zero'
 *           |
 *           v
 *         Gérer les signaux de processus avec 'handle_process_signal'
 *           |
 *           v
 *         Exécuter les commandes avec 'manage_exec_linked_cmd_sequence'
 *           |
 *           v
 *         'manage_exec_linked_cmd_sequence' a réussi ?
 *          /       \
 *        NON       OUI
 *         |         |
 *         |         |
 *         v         v
 *       Afficher   Est-ce une commande intégrée unique ?
 *       l'erreur    avec 'is_built_in_command'
 *          |        et 'data->utils->total_number_of_cmd_find_in_linked_list'
 *          |        /              \
 *          |      NON             OUI
 *          |        |              |
 *          |        |              |
 *          |        |              v
 *          |        v              Fermer les descripteurs
 *          |    Attendre les       de fichiers avec 
 *          |    processus          'close_file_descriptors_in_linked_list'
 *          |    enfants avec
 *          |    'wait_and_process_child_statuses'
 *          |        |
 *          |        v
 *          |    Fermer les descripteurs
 *          |    de fichiers avec 'close_file_descriptors_in_linked_list'
 *          |
 *          v
 *        Terminer
 *
 */
void	manage_execution_resources(t_data *data)
{
	int		wstatus;
	pid_t	*pid;
	int		fd[2];
	int		y[2];

	init_fd_and_status_vars(fd, y, &wstatus);
	pid = ft_malloc_with_tracking(\
	data, sizeof(pid_t) * (data->utils->cmd_count_pipe_chained));
	if (!pid)
		return ;
	initialize_pid_array_to_zero(pid, data->utils->cmd_count_pipe_chained);
	handle_process_signal();
	if (!manage_exec_linked_cmd_sequence(fd, pid, data, y))
		perror("Pipe ");
	wait_and_process_child_statuses(\
	pid, &wstatus, data->utils->cmd_count_pipe_chained, data);
	close_file_descriptors_in_linked_list(data);
}
