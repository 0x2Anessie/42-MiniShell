
#include "../../include/minishell.h"

/**
 * @nom: get_path
 * @brief: Extrait les chemins du répertoire à partir de la variable d'environnement PATH.
 *
 * @description:
 *   'get_path' parcourt la liste des variables d'environnement (t_env) pour trouver la variable PATH.
 *   Une fois trouvée, la fonction divise la valeur de PATH en un tableau de chaînes de caractères, 
 *   chacune représentant un chemin de répertoire où les commandes peuvent résider.
 *
 * @paramètres:
 *   - env_lst: t_env *env_lst, pointeur vers la liste des variables d'environnement.
 *
 * @pourquoi:
 *   - Localisation des Commandes : Facilite la recherche des commandes dans les répertoires listés 
 *     dans PATH, permettant au shell de trouver et d'exécuter des commandes situées hors du répertoire courant.
 *   - Gestion des Environnements : Permet une adaptation dynamique aux différents environnements 
 *     et configurations système où les chemins des commandes peuvent varier.
 *
 * @valeur_de_retour: 
 *   Retourne un tableau de chaînes de caractères contenant les chemins du répertoire PATH, ou NULL si PATH n'est pas trouvé.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Retourne NULL si la variable PATH n'est pas présente dans 'env_lst' ou si 'env_lst' est NULL.
 *
 * @exemples_d'utilisation:
 *   t_env *env_lst = load_env_variables();
 *   char **paths = get_path(env_lst); // paths = ["/bin", "/usr/bin", ...] si PATH est défini
 *
 * @dependances: 
 *   - 'ft_strncmp' pour comparer les chaînes de caractères.
 *   - 'ft_split' pour diviser la chaîne de caractères PATH en chemins individuels.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   'env_lst' est NULL ou 'env_lst->content' est NULL ?
 *  /        \
 * VRAI      FAUX
 *  |          \
 *  |           \
 *  v            v
 * Retourner   Parcourir 'env_lst'
 * NULL                |
 *                     |
 *                     v
 *            Trouvé 'PATH_PREFIX' dans 'env_lst->content' ?
 *           /                \
 *         VRAI              FAUX
 *          |                   \
 *          |                    \
 *          v                     v
 *       Diviser et              Passer au
 *       retourner               'env_lst->next'
 *       'path'
 *          |
 *          v
 *       Retourner 'path'
 */
char	**get_path(t_env *env_lst, t_data *data)
{
	char	**path;

	path = NULL;
	if (!env_lst || !env_lst->content)
	{
		return (NULL);
	}
	while (env_lst)
	{
		if (!ft_strncmp(env_lst->content, PATH_PREFIX, ft_strlen(PATH_PREFIX)))
		{
			path = ft_split_mini(env_lst->content + ft_strlen(PATH_PREFIX), ':', data);
			return (path);
		}
		env_lst = env_lst->next;
	}
	return (path);
}

/**
 * @nom: get_cmd_path
 * @brief: Récupère le chemin complet d'une commande en utilisant les variables d'environnement PATH.
 *
 * @description:
 *   'get_cmd_path' prend le nom d'une commande et cherche son chemin complet en utilisant la variable d'environnement PATH.
 *   La fonction vérifie si la commande est accessible directement ou doit être recherchée dans les répertoires spécifiés par PATH.
 *
 * @paramètres:
 *   - cmd: char *cmd, le nom de la commande à chercher.
 *   - env_lst: t_env *env_lst, la liste des variables d'environnement.
 *
 * @pourquoi:
 *   - Résolution de Commande : Permet de localiser les commandes dans les répertoires spécifiés par PATH, 
 *     essentiel pour exécuter des commandes qui ne sont pas dans le répertoire courant.
 *   - Accès et Exécution : Assure que les commandes sont accessibles et exécutables, prévenant les tentatives 
 *     d'exécution de commandes non valides.
 *
 * @valeur_de_retour: 
 *   Retourne le chemin complet de la commande si trouvé, sinon NULL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Retourne NULL si la commande n'est pas trouvée ou si l'accès n'est pas autorisé.
 *
 * @exemples_d'utilisation:
 *   t_env *env_lst = load_env_variables();
 *   char *path = get_cmd_path("ls", env_lst); // path = "/bin/ls" si trouvé
 *
 * @dependances: 
 *   - 'access' pour vérifier l'accessibilité des fichiers.
 *   - 'get_path' pour récupérer les répertoires PATH.
 *   - 'ft_strjoin2' pour concaténer les chaînes de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   'cmd' valide et exécutable ?
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * Retourner  Chemin d'accès par '/'
 * 'cmd' ?    /        \
 *           VRAI      FAUX
 *            |         \
 *            |          \
 *            v           v
 *          Retourner   Parcourir les chemins
 *          NULL        d'accès dans 'env_lst'
 *                        |
 *                        v
 *                      Trouver 'cmd' dans 'path' ?
 *                     /        \
 *                   VRAI      FAUX
 *                    |         \
 *                    |          \
 *                    v           v
 *                  Retourner   Continuer avec
 *                  chemin complet  le prochain 'path'
 *                    |
 *                    v
 *                  Retourner NULL
 */
char	*get_cmd_path(char *cmd, t_env *env_lst, t_data *data)
{
	int		index;
	char	*tmp;
	char	**path;

	index = ZERO_INIT;
	if (!cmd | !cmd[0])
		return (NULL);
	if (!access(cmd, X_OK))
		return (cmd);
	else if (access(cmd, X_OK) && *cmd == '/')
		return (NULL);
	path = get_path(env_lst, data);
	if (path)
	{
		while (path && path[index])
		{
			tmp = ft_strjoin2_mini(path[index], "/", data);
			tmp = ft_strjoin2_mini(tmp, cmd, data);
			if (!access(tmp, F_OK))
				return (tmp);
			index++;
		}	
	}
	return (NULL);
}

/**
 * @nom: check_path_exec
 * @brief: Vérifie si le chemin d'exécution d'une commande est valide.
 *
 * @description:
 *   'check_path_exec' examine le premier mot d'une structure lexer (t_lexer) pour déterminer si 
 *   le chemin de la commande spécifiée est valide en vérifiant si elle existe dans les variables 
 *   d'environnement. Si le chemin n'est pas trouvé, un message d'erreur est affiché.
 *
 * @paramètres:
 *   - lexer: t_lexer *lexer, pointeur vers la structure lexer contenant la commande à vérifier.
 *   - utils: t_exec utils, structure contenant les informations d'exécution, y compris les variables d'environnement.
 *
 * @pourquoi:
 *   - Validation de Commande : Assure que la commande spécifiée est accessible et exécutable 
 *     avant de tenter de l'exécuter, évitant ainsi les erreurs d'exécution.
 *   - Gestion d'Erreurs : Fournit des retours immédiats et informatifs en cas de chemin de 
 *     commande invalide, améliorant la robustesse du shell.
 *
 * @valeur_de_retour: 
 *   Retourne EXIT_SUCCESS si le chemin est valide, sinon EXIT_FAIL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Affiche un message d'erreur sur STDERR si le chemin n'est pas valide.
 *
 * @exemples_d'utilisation:
 *   t_lexer *lexer = create_lexer_list("ls -l");
 *   t_exec utils;
 *   int status = check_path_exec(lexer, utils); // status = EXIT_SUCCESS si 'ls' est valide
 *
 * @dependances: 
 *   - 'get_cmd_path' pour vérifier l'existence du chemin dans les variables d'environnement.
 *   - 'write' pour afficher des messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la validité du chemin (get_cmd_path)
 *     |
 *     v
 *   Chemin valide ?
 *  /       \
 * OUI      NON
 *  |         |
 *  v         v
 * Retourner  Afficher message d'erreur
 * EXIT_SUCCESS  (lexer->word + ERR_MSG_CMD_NOT_FOUND)
 *               |
 *               v
 *             Retourner EXIT_FAIL
 *               |
 *               v
 *             Fin
 */
int	check_path_exec(t_lexer *lexer, t_exec utils, t_data *data)
{
	if (!get_cmd_path(lexer->word, utils.env_lst, data))
	{
		write(STDERR_FILENO, lexer->word, strlen2(lexer->word));
		write(STDERR_FILENO, ERR_MSG_CMD_NOT_FOUND, ft_strlen(ERR_MSG_CMD_NOT_FOUND));
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

/**
 * @nom: get_arg
 * @brief: Construit un tableau de chaînes de caractères à partir des arguments dans une liste de lexèmes.
 *
 * @description:
 *   La fonction 'get_arg' parcourt une liste de lexèmes (t_lexer), collecte les mots qui sont marqués
 *   comme des arguments (ARG), et les stocke dans un tableau de chaînes de caractères. Ce tableau est
 *   utilisé pour passer les arguments aux commandes lors de leur exécution dans un shell.
 *
 * @paramètres:
 *   - lexer_list: t_lexer *lexer_list, pointeur vers le début de la liste de lexèmes.
 *
 * @pourquoi:
 *   - Construction d'Arguments : Permet de collecter et de structurer les arguments d'une commande
 *     en vue de leur utilisation lors de l'exécution de cette commande.
 *   - Simplification de l'Exécution : Fournit un moyen simple et structuré d'accéder aux arguments
 *     d'une commande, ce qui est essentiel pour le traitement et l'exécution des commandes dans un shell.
 *
 * @valeur_de_retour: 
 *   Retourne un tableau de chaînes de caractères contenant les arguments de la commande.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - En cas d'échec d'allocation mémoire avec 'ft_malloc_with_tracking', retourne NULL.
 *
 * @exemples_d'utilisation:
 *   t_lexer *lexer_list = create_lexer_list("cmd arg1 arg2");
 *   char **args = get_arg(lexer_list); // args contiendra {"arg1", "arg2", NULL}
 *
 * @dependances: 
 *   - 'ft_nb_arg' pour déterminer le nombre d'arguments.
 *   - 'ft_malloc_with_tracking' pour l'allocation mémoire sécurisée.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Calculer nombre d'arguments (ft_nb_arg)
 *     |
 *     v
 *   Allouer mémoire pour tableau d'arguments
 *     |
 *     v
 *   Initialiser tableau avec premier mot
 *     |
 *     v
 *   Parcourir 'lexer_list' jusqu'à ARG ou PIPE
 *     |
 *     v
 *   'lexer_list' est NULL ou token == PIPE ?
 *  /                      \
 * NON                     OUI
 *  |                       |
 *  v                       |
 * Incrémenter index,       |
 * passer au mot suivant    |
 *     |--------------------/
 *     v
 *   Remplir tableau avec mots ARG
 *     |
 *     v
 *   'lexer_list' est NULL ou index >= nb_arg ?
 *  /                       \
 * NON                      OUI
 *  |                        |
 *  v                        |
 * Ajouter mot à tableau,    |
 * incrémenter index         |
 *     |---------------------/
 *     v
 *   Marquer fin du tableau avec NULL
 *     |
 *     v
 *   Retourner tableau d'arguments
 *     |
 *     v
 *   Fin
 */

char	**get_arg(t_data *data)
{
	char	**arg;
	int		nb_arg;
	int		index;

	index = ZERO_INIT;
	nb_arg = ft_nb_arg(data->lexer_list);
	arg = ft_malloc_with_tracking(data, sizeof(char *) * (nb_arg + sizeof('\0')));
	arg[index] = data->lexer_list->word;
	while (data->lexer_list && data->lexer_list->token != ARG && data->lexer_list->token != PIPE)
		data->lexer_list = data->lexer_list->next;
	index++;
	while (data->lexer_list != NULL && index < nb_arg && data->lexer_list->token == ARG)
	{
		arg[index] = data->lexer_list->word;
		data->lexer_list = data->lexer_list->next;
		index++;
	}
	arg[index] = NULL;
	return (arg);
}

/**
 * @nom: ft_child
 * @brief: Crée un processus enfant pour exécuter une commande.
 *
 * @description:
 *   La fonction 'ft_child' utilise 'fork' pour créer un nouveau processus enfant. Si le processus
 *   enfant est créé avec succès, il exécute la commande spécifiée dans 'lexer_list' en appelant
 *   'child_of_chill'. Cette fonction est utilisée dans les environnements de shell pour gérer
 *   l'exécution de commandes dans des processus séparés.
 *
 * @paramètres:
 *   - lexer_list: t_lexer *lexer_list, pointeur vers la liste de lexèmes représentant la commande à exécuter.
 *   - fd: int *fd, tableau de descripteurs de fichier pour la gestion des pipes.
 *   - count: int count, l'index de la commande en cours dans la séquence de commandes.
 *   - utils: t_exec utils, structure contenant les informations sur l'exécution de la commande.
 *
 * @pourquoi:
 *   - Séparation des Processus : Créer un processus enfant pour chaque commande permet une 
 *     exécution isolée et sécurisée, évitant les interférences entre les commandes.
 *   - Gestion Flexible des Commandes : Permet au shell de continuer à recevoir et à traiter 
 *     d'autres commandes pendant que la commande actuelle est en cours d'exécution.
 *
 * @valeur_de_retour: 
 *   Retourne l'ID du processus enfant (pid_t) en cas de succès, ou FT_FAILURE_EXIT en cas d'échec de 'fork'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - En cas d'échec de 'fork', un message d'erreur est affiché et la fonction retourne une valeur d'échec.
 *
 * @exemples_d'utilisation:
 *   t_lexer *lexer_list = create_lexer_list("ls -l");
 *   int fd[2];
 *   t_exec utils;
 *   pid_t child_pid = ft_child(lexer_list, fd, 0, utils);
 *
 * @dependances: 
 *   - Utilise 'fork' pour créer un processus enfant.
 *   - 'child_of_chill' pour l'exécution de la commande dans le processus enfant.
 *   - 'perror' pour afficher les erreurs.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Créer un processus enfant (fork)
 *     |
 *     v
 *   'fork' a-t-il réussi ?
 *  /       \
 * NON      OUI
 *  |         |
 *  v         v
 * Afficher  Est-ce le processus enfant ?
 * un message  /       \
 * d'erreur  OUI      NON
 *  |         |         |
 *  v         v         v
 * Retourner  Exécuter  Retourner l'ID
 * FT_FAILURE_EXIT  la commande  du processus enfant
 *               avec child_of_chill
 *                    |
 *                    v
 *                  Fin
 */
pid_t	ft_child(t_data *data, int *fd, int count, t_exec utils)
{
	pid_t	pid;

	pid = fork();
	if (pid == FORK_FAILURE)
	{
		perror("Fork");
		return (FT_FAILURE_EXIT);
	}
	if (pid == CHILD_PROCESS)
	{
		child_of_chill(data, fd, count, utils);
	}
	return (pid);
}
