#include "../../include/minishell.h"

/**
 * @nom: extract_paths_from_env
 * @brief: Extrait les chemins du répertoire à partir de la variable
 * d'environnement PATH.
 *
 * @description:
 * 'extract_paths_from_env' parcourt la liste des variables d'environnement
 * (t_env) pour trouver la variable PATH. Une fois trouvée, la fonction divise
 * la valeur de PATH en un tableau de chaînes de caractères, chacune
 * représentant un chemin de répertoire où les commandes peuvent résider.
 *
 * @parametres:
 * - env_lst: t_env *env_lst, pointeur vers la liste des variables
 * d'environnement.
 *
 * @pourquoi:
 * - Localisation des Commandes : Facilite la recherche des commandes dans les
 * répertoires listés dans PATH, permettant au shell de trouver et d'exécuter
 * des commandes situées hors du répertoire courant.
 * - Gestion des Environnements : Permet une adaptation dynamique aux
 * différents environnements et configurations système où les chemins des
 * commandes peuvent varier.
 *
 * @valeur_de_retour: 
 * Retourne un tableau de chaînes de caractères contenant les chemins du
 * répertoire PATH, ou NULL si PATH n'est pas trouvé.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Retourne NULL si la variable PATH n'est pas présente dans 'env_lst' ou si
 * 'env_lst' est NULL.
 *
 * @exemples_utilisation:
 * t_env *env_lst = load_env_variables();
 * char **paths = extract_paths_from_env(env_lst);
 * -------> paths = ["/bin", "/usr/bin", ...] si PATH est défini
 *
 * @dependances: 
 * - 'ft_strncmp' pour comparer les chaînes de caractères.
 * - 'ft_split' pour diviser la chaîne de caractères PATH en chemins
 *   individuels.
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
 *			VRAI              FAUX
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
char	**extract_paths_from_env(t_env *env_lst, t_data *data)
{
	char	**path;

	path = NULL;
	if (!env_lst || !env_lst->var_env_name_and_value)/*         ---> condition non intelligible --> fonction         */
	{
		return (NULL);
	}
	while (env_lst)
	{
		if (!ft_strncmp(env_lst->var_env_name_and_value, ENV_SET_PATH_PREFIX, ft_strlen(ENV_SET_PATH_PREFIX)))/*         ---> condition non intelligible --> fonction         */
		{
			path = ft_split_mini(\
			env_lst->var_env_name_and_value + ft_strlen(ENV_SET_PATH_PREFIX), ':', data);
			return (path);
		}
		env_lst = env_lst->next_var_env_name_and_value;
	}
	return (path);
}

/**
 * @brief Vérifie si un chemin direct spécifié pour une commande est invalide.
 *
 * @param cmd Le chemin de la commande à vérifier.
 * @return true Si le chemin direct est invalide (non exécutable et commence par un '/'), sinon false.
 */
bool is_invalid_direct_path(const char *cmd)
{
    return (access(cmd, X_OK) && *cmd == '/');
}


/**
 * @nom: find_command_full_path
 * @brief: Récupère le chemin complet d'une commande en utilisant les variables
 * d'environnement PATH.
 *
 * @description:
 * 'find_command_full_path' prend le nom d'une commande et cherche son chemin
 * complet en utilisant la variable d'environnement PATH. La fonction vérifie
 * si la commande est accessible directement ou doit être recherchée dans les
 * répertoires spécifiés par PATH.
 *
 * @parametres:
 * - cmd: char *cmd, le nom de la commande à chercher.
 * - env_lst: t_env *env_lst, la liste des variables d'environnement.
 *
 * @pourquoi:
 * - Résolution de Commande : Permet de localiser les commandes dans les
 * répertoires spécifiés par PATH, essentiel pour exécuter des commandes qui ne
 * sont pas dans le répertoire courant.
 * - Accès et Exécution : Assure que les commandes sont accessibles et
 * exécutables, prévenant les tentatives d'exécution de commandes non valides.
 *
 * @valeur_de_retour: 
 * Retourne le chemin complet de la commande si trouvé, sinon NULL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Retourne NULL si la commande n'est pas trouvée ou si l'accès n'est pas
 * autorisé.
 *
 * @exemples_utilisation:
 * t_env *env_lst = load_env_variables();
 * char *path = find_command_full_path("ls", env_lst); // path = "/bin/ls" si
 * trouvé
 *
 * @dependances: 
 * - 'access' pour vérifier l'accessibilité des fichiers.
 * - 'extract_paths_from_env' pour récupérer les répertoires PATH.
 * - 'ft_strjoin2' pour concaténer les chaînes de caractères.
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
char	*find_command_full_path(char *cmd, t_env *env_lst, t_data *data)
{
	int		index;
	char	*tmp;
	char	**path;

	index = ZERO_INIT;
	if (!cmd | !cmd[0])/*         ---> condition non intelligible --> fonction         */
		return (NULL);
	if (!access(cmd, X_OK))/*         ---> condition non intelligible --> fonction         */
		return (cmd);
	else if (is_invalid_direct_path(cmd))
		return (NULL);
	path = extract_paths_from_env(env_lst, data);
	if (path)
	{
		while (path && path[index])/*         ---> condition non intelligible --> fonction         */
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
 * @nom: check_if_cmd_full_path_exec_is_valid
 * @brief: Vérifie si le chemin d'exécution d'une commande est valide.
 *
 * @description:
 * 'check_if_cmd_full_path_exec_is_valid' examine le premier mot d'une
 * lexer (t_lexer) pour déterminer si le chemin de la commande spécifiée est
 * valide en vérifiant si elle existe dans les variables d'environnement. Si
 * le chemin n'est pas trouvé, un message d'erreur est affiché.
 *
 * @parametres:
 * - lexer: t_lexer *lexer, pointeur vers la structure lexer contenant la
 * commande à vérifier.
 * - utils: t_exec utils, structure contenant les informations d'exécution, y
 * compris les variables d'environnement.
 *
 * @pourquoi:
 * - Validation de Commande : Assure que la commande spécifiée est accessible
 * et exécutable avant de tenter de l'exécuter, évitant ainsi les erreurs
 * d'exécution.
 * - Gestion d'Erreurs : Fournit des retours immédiats et informatifs en cas
 * de chemin de commande invalide, améliorant la robustesse du shell.
 *
 * @valeur_de_retour: 
 * Retourne EXIT_SUCCESS si le chemin est valide, sinon EXIT_FAIL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Affiche un message d'erreur sur STDERR si le chemin n'est pas valide.
 *
 * @exemples_utilisation:
 * t_lexer *lexer = create_lexer_list("ls -l");
 * t_exec utils;
 * int status = check_if_cmd_full_path_exec_is_valid(lexer, utils);
 * -------> status = EXIT_SUCCESS si 'ls' est valide
 *
 * @dependances: 
 * - 'find_command_full_path' pour vérifier l'existence du chemin dans les
 * variables d'environnement.
 * - 'write' pour afficher des messages d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier la validité du chemin (find_command_full_path)
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
int	check_if_cmd_full_path_exec_is_valid(\
t_lexer *lexer, t_exec utils, t_data *data)
{
	if (!find_command_full_path(lexer->cmd_segment, utils.linked_list_full_env_var_copy_alpha, data))
	{
		write(STDERR_FILENO, lexer->cmd_segment, strlen2(lexer->cmd_segment));
		write(\
		STDERR_FILENO, ERR_MSG_CMD_NOT_FOUND, ft_strlen(ERR_MSG_CMD_NOT_FOUND));
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

bool	is_lexer_token_cmd_arg(t_lexer *lexer_item)
{
    return (lexer_item != NULL && lexer_item->token == ARG);
}

/**
 * @brief Vérifie si l'itération dans la liste des lexèmes doit continuer.
 *
 * @param lexer_item L'élément actuel du lexer à vérifier.
 * @param current_index L'indice courant dans le processus d'itération.
 * @param total_args Nombre total d'arguments attendus.
 * @return true si l'itération doit continuer, sinon false.
 */
bool should_continue_lexer_iteration(t_lexer *lexer_item, int current_index, int total_args)
{
    return (lexer_item->next != NULL && current_index < total_args);
}

char **build_arg_array_from_lexer(t_data *data)
{
    char **arg;
    int nb_arg;
    int index;

    index = ZERO_INIT;
    nb_arg = count_args_until_pipe_for_cmd_array(data->lexer_list);
    arg = ft_malloc_with_tracking(data, sizeof(char *) * (nb_arg + 1));
    arg[index++] = data->lexer_list->cmd_segment;
    while (should_continue_lexer_iteration(data->lexer_list, index, nb_arg))
    {
        if (is_lexer_token_cmd_arg(data->lexer_list->next))
            arg[index++] = data->lexer_list->next->cmd_segment;
        data->lexer_list->next = data->lexer_list->next->next;
    }
    arg[index] = NULL;
    return arg;
}

