#include "../../include/minishell.h"

int	check_for_slash_path_delimiter(t_lexer *lexer)
{
	int		indx;

	indx = ZERO_INIT;
	while (lexer->cmd_segment[indx])
	{
		if (lexer->cmd_segment[indx++] == '/')
			return (SLASH_FOUND);
	}
	return (NO_SLASH_FOUND);
}

void	execute_lexer_command_with_args(t_data *data)
{
	char	**arguments;

	arguments = build_arg_array_from_lexer(data);
	if (is_execve_failed(data->lexer_list->cmd_segment, arguments, \
	data->utils->full_env_var_copy_beta))
		perror(data->lexer_list->cmd_segment);
}

void	exec_cmd_with_redirection_and_builtins(\
t_data *data, int *fd, int count, t_exec utils)
{
	char	*command_full_path;
	char	**arguments;

	configure_stdin_redirection_for_command(utils, count);
	configure_stdout_redirection_for_command(utils, fd, count);
	if (is_built_in_command(data->lexer_list))
	{
		ft_exec_single_built_in(data->lexer_list, fd, data);
		ft_exit_child(fd, data);
	}
	if (!check_for_slash_path_delimiter(data->lexer_list))
		execute_lexer_command_with_args(data);
	else if (!check_if_cmd_full_path_exec_is_valid(\
	data->lexer_list, utils, data))
	{
		command_full_path = find_command_full_path(data->lexer_list->\
		cmd_segment, utils.linked_list_full_env_var_copy_alpha, data);
		arguments = build_arg_array_from_lexer(data);
		if (is_execve_failed(\
		command_full_path, arguments, utils.full_env_var_copy_beta))
			perror(data->lexer_list->cmd_segment);
	}
	g_signal_received = ERR_CODE_CMD_NOT_FOUND;
	ft_exit_child(fd, data);
}

/**
 * @nom: create_and_execute_child_process
 * @brief: Crée un processus enfant pour exécuter une commande.
 *
 * @description:
 * La fonction 'create_and_execute_child_process' utilise 'fork' pour créer un
 * nouveau processus enfant. Si le processus enfant est créé avec succès, il
 * exécute la commande spécifiée dans 'lexer_list' en appelant
 * 'exec_cmd_with_redirection_and_builtins'. Cette fonction est utilisée dans
 * les environnements de shell pour gérer l'exécution de commandes dans des
 * processus séparés.
 *
 * @parametres:
 * - lexer_list: t_lexer *lexer_list, pointeur vers la liste de lexèmes
 * représentant la commande à exécuter.
 * - fd: int *fd, tableau de descripteurs de fichier pour la gestion des pipes.
 * - count: int count, l'index de la commande en cours dans la séquence de
 * commandes.
 * - utils: t_exec utils, structure contenant les informations sur l'exécution
 * de la commande.
 *
 * @pourquoi:
 * - Séparation des Processus : Créer un processus enfant pour chaque commande
 * permet une exécution isolée et sécurisée, évitant les interférences entre
 * les commandes.
 * - Gestion Flexible des Commandes : Permet au shell de continuer à recevoir
 * et à traiter d'autres commandes pendant que la commande actuelle est en
 * cours d'exécution.
 *
 * @valeur_de_retour: 
 * Retourne l'ID du processus enfant (pid_t) en cas de succès, ou
 * FT_FAILURE_EXIT en cas d'échec de 'fork'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - En cas d'échec de 'fork', un message d'erreur est affiché et la fonction
 * retourne une valeur d'échec.
 *
 * @exemples_utilisation:
 * t_lexer *lexer_list = create_lexer_list("ls -l");
 * int fd[2];
 * t_exec utils;
 * pid_t child_pid = create_and_execute_child_process(lexer_list, fd, 0, utils);
 *
 * @dependances: 
 * - Utilise 'fork' pour créer un processus enfant.
 * - 'exec_cmd_with_redirection_and_builtins' pour l'exécution de la commande
 *   dans le processus enfant.
 * - 'perror' pour afficher les erreurs.
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
 *               avec exec_cmd_with_redirection_and_builtins
 *                    |
 *                    v
 *                  Fin
 */
pid_t	create_and_execute_child_process(\
t_data *data, int *fd, int count, t_exec utils)
{
	pid_t	pid;

	pid = fork();
	if (pid == FORK_FAILURE)
	{
		perror("Fork");
		return (FT_FAILURE_EXIT);
	}
	if (pid == CHILD_PROCESS)
		exec_cmd_with_redirection_and_builtins(data, fd, count, utils);
	return (pid);
}
