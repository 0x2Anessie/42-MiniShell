#include "../../include/minishell.h"

/**
 * @nom: configure_stdin_redirection_for_command
 * @brief: Configure la redirection de l'entrée standard pour une commande.
 *
 * @description:
 * Cette fonction gère la redirection de l'entrée standard pour une commande
 * donnée. Elle redirige l'entrée standard vers un descripteur de fichier
 * spécifié, en se basant sur la configuration du nœud de commande actuel et la
 * position de la commande dans la séquence de commandes. C'est un composant
 * crucial dans la gestion des pipelines et des redirections d'entrée dans un
 * shell.
 *
 * @param utils: t_exec utils, structure contenant les informations d'exécution
 * et de gestion des nœuds.
 * @param count: int count, indice de la commande actuelle dans la séquence des
 * commandes.
 *
 * @pourquoi:
 * - Redirection de l'Entrée Standard : Cruciale pour assurer que l'entrée des
 * commandes provienne du bon descripteur de fichier, qu'il s'agisse d'un
 * pipeline ou d'un fichier de redirection.
 * - Gestion des Pipelines : Permet de connecter les entrées des commandes aux
 * sorties des commandes précédentes, facilitant la création de flux de données
 * complexes entre les commandes.
 * - Optimisation des Ressources : Assure que les descripteurs de fichier
 * inutilisés sont fermés après utilisation, prévenant les fuites de ressources
 * et les erreurs.
 *
 * @valeur_de_retour: 
 * Aucune (void). Effectue des opérations de redirection et de fermeture de
 * descripteurs.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Peut échouer si les appels système dup2 ou close échouent.
 *
 * @exemples_utilisation:
 * t_exec utils = {...};
 * -------> Initialisé avec les nœuds de commande et autres détails
 * int count = 1;
 * -------> Indice de la commande actuelle
 * configure_stdin_redirection_for_command(utils, count);
 *
 * @dependances: 
 * - dup2 et close pour la manipulation des descripteurs de fichier.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier si une redirection d'entrée est configurée
 *  /       \
 * OUI      NON
 *  |         |
 *  v         v
 * Rediriger  Vérifier si ce n'est pas la première commande
 * l'entrée   et si un descripteur de pipe précédent est disponible
 * à 'input_fd'     /         \
 *  |       OUI      NON
 *  v         |         |
 * Fermer     v         v
 * 'input_fd' et    Rediriger  Fin de la fonction
 * 'previous_fd'   l'entrée
 *  |         à 'previous_fd'
 *  v
 * Fin
 */
void	configure_stdin_redirection_for_command(t_exec utils, int count)
{
	if (utils.node->input_fd >= 0)/*         ---> condition non intelligible --> fonction         */
	{
		dup2(utils.node->input_fd, STDIN_FILENO);
		if (utils.node->input_fd > 0)/*         ---> condition non intelligible --> fonction         */
			close(utils.node->input_fd);
		if (utils.previous_fd)/*         ---> condition non intelligible --> fonction         */
			close(utils.previous_fd);
	}
	else if (count)
	{
		dup2(utils.previous_fd, STDIN_FILENO);
		if (utils.previous_fd > 0)/*         ---> condition non intelligible --> fonction         */
			close(utils.previous_fd);
	}
}

/**
 * @nom: configure_stdout_redirection_for_command
 * @brief: Gère la redirection de la sortie standard pour une commande.
 *
 * @description:
 * Redirige la sortie standard vers un descripteur de fichier spécifié, en
 * fonction de la configuration du nœud de commande actuel et de la position de
 * la commande dans la séquence globale des commandes. Cette fonction est
 * utilisée pour rediriger correctement la sortie des commandes dans un
 * pipeline ou vers un fichier.
 *
 * @param utils: t_exec utils, structure contenant les informations d'exécution
 * et de gestion des nœuds.
 * @param fd: int *fd, tableau de descripteurs de fichier pour la communication
 * entre les commandes.
 * @param count: int count, indice de la commande actuelle dans la séquence des
 * commandes.
 *
 * @pourquoi:
 * - Gestion des Redirections de Sortie : Essentielle pour rediriger la sortie
 * des commandes vers le bon emplacement, que ce soit un fichier ou le
 * descripteur de fichier d'une autre commande dans un pipeline.
 * - Flexibilité dans les Pipelines : Permet de connecter dynamiquement les
 * sorties des commandes aux entrées des commandes suivantes, facilitant la
 * création de pipelines complexes.
 * - Prévention des Fuites de Ressources : S'assure que les descripteurs de
 * fichier inutilisés sont fermés pour éviter les fuites de ressources et les
 * erreurs potentielles.
 *
 * @valeur_de_retour: 
 * Aucune (void). La fonction effectue des opérations de redirection et de
 * fermeture de descripteurs.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - La fonction peut échouer si les appels système dup2 ou close échouent.
 *
 * @exemples_utilisation:
 * t_exec utils = {...};
 * -------> Initialisé avec les nœuds de commande et autres détails
 * int fd[2] = {pipefd[0], pipefd[1]};
 * -------> Descripteurs de fichier pour un pipeline
 * int count = 0;
 * -------> Indice de la commande actuelle
 * configure_stdout_redirection_for_command(utils, fd, count);
 *
 * @dependances: 
 * - dup2 et close pour la gestion des descripteurs de fichier.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier si une redirection de sortie est configurée
 *  /       \
 * OUI      NON
 *  |         |
 *  v         v
 * Rediriger  Vérifier si c'est la dernière commande
 * la sortie  et si un descripteur de pipe est disponible
 * à 'output_fd'    /         \
 *  |              OUI      NON
 *  v               |         |
 * Fermer           v         v
 * 'input_fd' si    Rediriger  Fin de la fonction
 * ouvert           la sortie
 *  |               à fd[1]
 *  v
 * Fermer fd[1] et fd[0] s'ils sont ouverts
 *  |
 *  v
 * Fin
 */
void	configure_stdout_redirection_for_command(\
t_exec utils, int *fd, int count)
{
	if (utils.node->output_fd > 0)/*         ---> condition non intelligible --> fonction         */
	{
		dup2(utils.node->output_fd, STDOUT_FILENO);
		if (utils.node->input_fd > 0)/*         ---> condition non intelligible --> fonction         */
			close (utils.node->input_fd);
	}
	else if (count != utils.nb_cmd - 1 && fd[1] > 0)/*         ---> condition non intelligible --> fonction         */
		dup2(fd[1], STDOUT_FILENO);
	if (fd[1] > 0)/*         ---> condition non intelligible --> fonction         */
		close(fd[1]);
	if (fd[0] > 0)/*         ---> condition non intelligible --> fonction         */
		close (fd[0]);
}
