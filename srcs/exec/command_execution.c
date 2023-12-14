
#include "../../include/minishell.h"

/**
 * @fonction: ft_exec_single_built_in
 * @brève_description: Exécute une commande intégrée unique à partir d'une liste de lexèmes.
 *
 * @param lexer_lst: Pointeur sur t_lexer, la liste des lexèmes à analyser.
 * @param fd: Pointeur sur un entier représentant un descripteur de fichier.
 *
 * @description_détaillée:
 *   La fonction 'ft_exec_single_built_in' a pour rôle d'exécuter une commande intégrée spécifique,
 *   identifiée dans la liste des lexèmes ('lexer_lst'). Elle utilise 'is_command_equal' pour comparer
 *   le mot actuel avec une série de commandes intégrées connues (comme 'export', 'pwd', 'echo', etc.),
 *   et appelle la fonction appropriée pour exécuter la commande.
 *
 *   Pourquoi exécuter des commandes intégrées individuellement ?
 *   - Modularité : Chaque commande intégrée peut nécessiter une logique différente. En les traitant
 *     individuellement, on maintient une structure claire et modulaire.
 *   - Séparation des responsabilités : Cette approche permet de déléguer la responsabilité de l'exécution
 *     à des fonctions spécialisées, améliorant ainsi la lisibilité et la maintenance du code.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'lexer_lst' est NULL, les fonctions spécifiques aux commandes intégrées ne seront pas appelées.
 *   - La fermeture du descripteur de fichier (si 'g_all.utils->node->out > 0') doit être gérée avec soin pour
 *     éviter des fuites de ressources.
 *
 * @exemples_d'utilisation:
 *   t_lexer lexer;
 *   int fd = 1;
 *   lexer.word = "export";
 *   ft_exec_single_built_in(&lexer, &fd);
 *
 * @dépendances:
 *   - is_command_equal() : Pour comparer les commandes.
 *   - export_things(), get_pwd(), init_echo(), etc. : Fonctions spécifiques aux commandes intégrées.
 *   - g_all.utils->node : Pour gérer les descripteurs de fichiers.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la commande dans lexer_lst (export, pwd, echo, cd, env, unset, exit)
 *  /        \        ...      ...      ...      ...      ...      ...      \
 * CMD      CMD       ...      ...      ...      ...      ...      ...      CMD
 * EXPORT   PWD       ...      ...      ...      ...      ...      ...      EXIT
 *  |         |        ...      ...      ...      ...      ...      ...      |
 *  |         |        ...      ...      ...      ...      ...      ...      |
 *  v         v        ...      ...      ...      ...      ...      ...      v
 * Exécuter  Exécuter  ...      ...      ...      ...      ...      ...   Exécuter
 * export    pwd       ...      ...      ...      ...      ...      ...    exit
 *            |
 *            |
 *            v
 *        Fermer le descripteur de fichier si nécessaire
 *            |
 *            v
 *        Réinitialiser lexer_lst à la tête de la liste
 */
void ft_exec_single_built_in(t_lexer *lexer_lst, int *fd, t_data *data)
{
    if (is_command_equal(lexer_lst, CMD_EXPORT_VARS, strlen(CMD_EXPORT_VARS)))
        export_things(lexer_lst, data);
    else if (is_command_equal(lexer_lst, CMD_PRINT_DIRCT, strlen(CMD_PRINT_DIRCT)))
        get_pwd(lexer_lst->word, data);
    else if (is_command_equal(lexer_lst, CMD_ECHO, strlen(CMD_ECHO)))
        init_echo(lexer_lst, data);
    else if (is_command_equal(lexer_lst, CMD_CHANG_DIRCT, strlen(CMD_CHANG_DIRCT)))
        get_cd(lexer_lst, data);
    else if (is_command_equal(lexer_lst, CMD_ENV_VARS, strlen(CMD_ENV_VARS)))
        get_unset(lexer_lst);
    else if (is_command_equal(lexer_lst, CMD_UNSET_VARS, strlen(CMD_UNSET_VARS)))
        get_unset(lexer_lst);
    else if (is_command_equal(lexer_lst, CMD_EXIT_SHELL, strlen(CMD_EXIT_SHELL)))
        ft_exit(lexer_lst, fd, data);

    if (g_all.utils->node->out > 0)
        close(g_all.utils->node->out);
    lexer_lst = g_all.utils->head_lexer_lst;
}

/**
 * @fonction: close_fds_if_needed
 * @brève_description: Ferme les descripteurs de fichiers spécifiés si nécessaire.
 *
 * @param fd: Pointeur sur un tableau d'entiers contenant les descripteurs de fichiers à évaluer.
 * @param previous_fd: Entier représentant un descripteur de fichier supplémentaire à évaluer.
 *
 * @description_détaillée:
 *   La fonction 'close_fds_if_needed' examine et ferme les descripteurs de fichiers spécifiés
 *   si leurs valeurs indiquent qu'ils sont ouverts (c'est-à-dire, supérieurs à 0). Cette fonction
 *   est utilisée pour gérer correctement les ressources système en fermant les descripteurs de
 *   fichiers qui ne sont plus nécessaires.
 *
 *   Pourquoi la fermeture conditionnelle des descripteurs de fichiers est-elle importante ?
 *   - Prévention des fuites de ressources : La fermeture appropriée des descripteurs de fichiers
 *     évite les fuites de ressources, ce qui est crucial pour la stabilité et les performances du système.
 *   - Bonne gestion des ressources : Assure que les descripteurs de fichiers sont libérés quand ils
 *     ne sont plus utilisés, permettant ainsi une utilisation plus efficace des ressources du système.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'fd' est NULL ou si 'fd[1]' n'est pas un descripteur de fichier valide, la fermeture peut échouer ou
 *     avoir un comportement indéfini.
 *   - La fermeture de 'previous_fd' peut échouer si ce n'est pas un descripteur de fichier valide.
 *
 * @exemples_d'utilisation:
 *   int fds[2] = {1, 2};
 *   int prev_fd = 3;
 *   close_fds_if_needed(fds, prev_fd); // Ferme fds[1] et prev_fd si ouverts
 *
 * @dépendances: Fonction 'close' de la bibliothèque standard pour la fermeture des descripteurs.
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
    if (fd[1] > 0)
        close(fd[1]);
    if (previous_fd > 0)
        close(previous_fd);
}

/**
 * @fonction: start_exec
 * @brève_description: Démarre l'exécution des commandes dans une liste de lexèmes.
 *
 * @param fd: Pointeur sur un tableau d'entiers pour les descripteurs de fichiers (pipes).
 * @param pid: Pointeur sur un tableau de pid_t pour stocker les PID des processus enfants.
 * @param lex_lst: Pointeur sur une structure t_lexer représentant la liste des lexèmes.
 * @param y: Pointeur sur un tableau d'entiers utilisé pour la logique de contrôle.
 *
 * @description_détaillée:
 *   La fonction 'start_exec' orchestre l'exécution des commandes en parcourant une liste de lexèmes. Elle
 *   initialise 'lex_lst' avec la tête de la liste de lexèmes globale, puis entre dans une boucle contrôlée
 *   par 'should_continue_execution'. Dans cette boucle, elle trouve la prochaine commande, gère les pipes,
 *   exécute des commandes intégrées ou crée des processus enfants pour les autres commandes, et gère
 *   les descripteurs de fichiers ouverts.
 *
 *   Pourquoi cette orchestration est-elle cruciale ?
 *   - Coordination des commandes : Assure que chaque commande est exécutée dans le bon ordre et
 *     avec les bons paramètres, en particulier dans un environnement avec pipes et redirections.
 *   - Gestion des ressources : La création et la fermeture correctes des pipes et des descripteurs
 *     de fichiers préviennent les fuites de ressources.
 *
 * @valeur_de_retour:
 *   Retourne 1 (succès) si toutes les commandes sont exécutées correctement, 0 (échec) en cas d'erreur.
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si une commande intégrée échoue ou si 'pipe(fd)' renvoie une erreur, la fonction retourne 0.
 *   - La modification des valeurs de 'lex_lst' et 'g_all.utils->node' a un effet sur l'état global du programme.
 *
 * @exemples_d'utilisation:
 *   int fd[2], y[2] = {0, 0};
 *   pid_t pid[10];
 *   t_lexer *lex_lst;
 *   int result = start_exec(fd, pid, lex_lst, y); // Résultat attendu : 1 ou 0
 *
 * @dépendances:
 *   - should_continue_execution, find_next_command, is_valid_redirection, etc. : Fonctions auxiliaires pour la logique d'exécution.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser lex_lst avec la tête de la liste de lexèmes globale (g_all.utils->head_lexer_lst)
 *   |
 *   v
 * Entrer dans la boucle tant que should_continue_execution retourne vrai
 *   |
 *   v
 * Trouver la prochaine commande avec find_next_command
 *   |
 *   v
 * Initialiser g_all.utils->previous_fd avec fd[0]
 *   |
 *   v
 * g_all.utils->nb_cmd >= 1 et pipe(fd) échoue ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner 0  Continuer l'exécution
 *   |
 *   v
 * is_valid_redirection de g_all.utils->node est vrai ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Continuer   Passer à la prochaine commande sans exécuter
 * l'exécution
 *   |
 *   v
 * (is_built_in(lex_lst)) et (g_all.utils->nb_cmd == 1) ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Exécuter   Créer un processus enfant avec ft_child et
 * ft_exec_single_built_in  stocker le PID dans pid[y[0]++]
 *   |
 *   v
 * Passer à la prochaine commande avec go_next_cmd
 *   |
 *   v
 * Fermer les descripteurs de fichiers ouverts avec close_fds_if_needed
 *   |
 *   v
 * Passer au nœud suivant dans g_all.utils->node
 *   |
 *   v
 * Retour à la condition de la boucle (should_continue_execution)
 *   |
 *   v
 * Fin de la boucle
 *   |
 *   v
 * Fermer les pipes restants avec close_pipe
 *   |
 *   v
 * Retourner 1
 * Fin
 */
int	start_exec(int *fd, pid_t *pid, t_data *data, int *y)
{
	data->lexer_list = g_all.utils->head_lexer_lst;
	while (should_continue_execution(&g_all, y))
	{
		data->lexer_list = find_next_command(data->lexer_list);
		g_all.utils->previous_fd = fd[0];
		if (g_all.utils->nb_cmd >= 1 && pipe(fd) < 0)
			return (0);
		if (is_valid_redirection(g_all.utils->node))
		{
			if ((is_built_in(data->lexer_list)) && g_all.utils->nb_cmd == 1)
				ft_exec_single_built_in(data->lexer_list, fd, data);
			else
				pid[y[0]++] = ft_child(data, fd, y[1], *(g_all.utils));
		}
		data->lexer_list = go_next_cmd(data->lexer_list);
		close_fds_if_needed(fd, g_all.utils->previous_fd);
		g_all.utils->node = g_all.utils->node->next;
	}
	close_pipe(fd);
	return (1);
}

/**
 * @fonction: wait_child_processes
 * @brève_description: Attend la terminaison des processus enfants spécifiés.
 *
 * @param pid: Pointeur sur un tableau de pid_t contenant les PID des processus enfants.
 * @param wstatus: Pointeur sur un entier pour stocker le statut de terminaison des processus.
 * @param nb_node: Entier représentant le nombre de processus enfants à attendre.
 *
 * @description_détaillée:
 *   La fonction 'wait_child_processes' parcourt un tableau de PID de processus enfants et attend
 *   leur terminaison. Pour chaque PID valide (supérieur à 0), elle appelle 'waitpid' pour attendre
 *   la fin du processus correspondant. Si un processus se termine normalement, elle met à jour
 *   'g_all.utils->err' avec le code de sortie du processus.
 *
 *   Pourquoi attendre les processus enfants ?
 *   - Prévention des processus zombies : Attendre les processus enfants permet de libérer les ressources
 *     associées et d'éviter les processus zombies qui pourraient encombrer le système.
 *   - Récupération des codes de sortie : Cela permet de récupérer les codes de sortie des processus enfants
 *     pour une gestion appropriée des erreurs ou des états de sortie.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'pid' est NULL, la fonction retourne immédiatement sans attendre aucun processus.
 *   - La fonction dépend de 'g_all.utils->can_run' et 'g_all.utils->nb_cmd' pour déterminer si elle
 *     doit continuer à attendre les processus.
 *
 * @exemples_d'utilisation:
 *   pid_t pids[10];
 *   int wstatus;
 *   int nb_node = 5;
 *   wait_child_processes(pids, &wstatus, nb_node); // Attend la fin de 5 processus enfants
 *
 * @dépendances:
 *   - waitpid() : Fonction de la bibliothèque standard pour attendre un processus enfant spécifique.
 *   - WIFEXITED et WEXITSTATUS : Macros pour analyser le statut renvoyé par waitpid.
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
 *   |       'g_all.utils->can_run' et 'g_all.utils->nb_cmd' sont vrais
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
 *   |         Passer à  Mettre à jour 'g_all.utils->err' avec WEXITSTATUS(*wstatus)
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
void	wait_child_processes(pid_t *pid, int *wstatus, int nb_node)
{
    int index;

	index = ZERO_INIT;
    if (!pid)
        return;
    while (nb_node > 0 && g_all.utils->can_run && g_all.utils->nb_cmd)
    {
        if (pid[index] > 0)
        {
            waitpid(pid[index], wstatus, 0);
            if (WIFEXITED(*wstatus))
                g_all.utils->err = WEXITSTATUS(*wstatus);
        }
        index++;
        nb_node--;
    }
}

/**
 * @fonction: ft_prep_exec
 * @brève_description: Prépare et exécute les commandes à partir des données fournies.
 *
 * @param data: Pointeur sur t_data contenant les données nécessaires à l'exécution.
 *
 * @description_détaillée:
 *   La fonction 'ft_prep_exec' coordonne l'exécution des commandes en initialisant les variables nécessaires,
 *   en allouant de la mémoire pour les PID des processus enfants, en gérant les signaux de processus, en
 *   démarrant l'exécution des commandes, et en attendant la fin des processus enfants. Elle gère également
 *   l'ouverture et la fermeture des descripteurs de fichiers.
 *
 *   Pourquoi cette préparation est-elle essentielle ?
 *   - Cohérence de l'exécution : Assure que toutes les variables et ressources nécessaires sont correctement
 *     initialisées avant l'exécution des commandes.
 *   - Gestion des ressources : La gestion de la mémoire et des signaux est cruciale pour le bon déroulement
 *     et la stabilité de l'exécution.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si l'allocation de 'pid' échoue, la fonction retourne sans exécuter les commandes.
 *   - Les appels à 'start_exec' et 'wait_child_processes' peuvent influencer l'état global et les ressources du système.
 *
 * @exemples_d'utilisation:
 *   t_data *data;
 *   ft_prep_exec(&data); // Prépare et exécute les commandes en fonction des données fournies dans 'data'
 *
 * @dépendances:
 *   - start_exec, wait_child_processes, close_fd, etc. : Fonctions utilisées pour l'exécution et la gestion des processus.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser les variables (fd, y, wstatus) avec 'init_var'
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
 * Retour   Initialiser 'pid' avec 'ft_bzero_pid_array'
 *           |
 *           v
 *         Gérer les signaux de processus avec 'handle_process_signal'
 *           |
 *           v
 *         Exécuter les commandes avec 'start_exec'
 *           |
 *           v
 *         'start_exec' a réussi ?
 *          /       \
 *        NON       OUI
 *         |         |
 *         |         |
 *         v         v
 *       Afficher   Est-ce une commande intégrée unique ?
 *       l'erreur    avec 'is_built_in' et 'g_all.utils->nb_cmd'
 *          |        /       \
 *          |      NON       OUI
 *          |        |         |
 *          |        |         |
 *          |        v         v
 *          |    Attendre les   Fermer les descripteurs
 *          |    processus      de fichiers avec 'close_fd'
 *          |    enfants avec
 *          |    'wait_child_processes'
 *          |        |
 *          |        v
 *          |    Fermer les descripteurs
 *          |    de fichiers avec 'close_fd'
 *          |
 *          v
 *        Terminer
 *
 */
void	ft_prep_exec(t_data *data)
{
	int		wstatus;
	pid_t	*pid;
	int		fd[2];
	int		y[2];

	init_var(fd, y, &wstatus);
	pid = ft_malloc_with_tracking(data, sizeof(pid_t) * (g_all.utils->nb_node));
	if (!pid)
		return ;
	ft_bzero_pid_array(pid, g_all.utils->nb_node);
	handle_process_signal();
	if (!start_exec(fd, pid, data, y))
		perror("Pipe ");
	// if (is_built_in(data->lexer_list) && g_all.utils->nb_cmd == 1 && close_fd())
	// 	return ;
	wait_child_processes(pid, &wstatus, g_all.utils->nb_node);
	close_fd();
}
