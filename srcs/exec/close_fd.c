
#include "../../include/minishell.h"

/**
 * @nom: close_pipe
 * @brief: Ferme les deux extrémités d'un pipe.
 *
 * @description:
 *   La fonction 'close_pipe' prend en argument un tableau de descripteurs de fichier représentant
 *   un pipe et ferme les deux extrémités de ce pipe. Elle est typiquement utilisée dans les environnements
 *   de shell pour la gestion des communications entre processus via des pipes.
 *
 * @param fd: int *fd, tableau contenant les descripteurs de fichier des deux extrémités du pipe.
 *
 * @pourquoi:
 *   - Gestion des Ressources : Assure que les ressources allouées pour le pipe sont correctement libérées,
 *     évitant ainsi les fuites de descripteurs de fichier.
 *   - Contrôle de Flux : Important pour terminer la communication entre les processus une fois que les 
 *     données nécessaires ont été transmises, contribuant à un fonctionnement correct et ordonné du shell.
 *
 * @valeur_de_retour: 
 *   Retourne 1 pour indiquer que la fermeture des extrémités du pipe a été effectuée.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction ne gère pas les erreurs éventuelles de la fonction 'close'.
 *
 * @exemples_d'utilisation:
 *   int fd[2];
 *   pipe(fd); // Création d'un pipe
 *   // ... Utilisation du pipe
 *   close_pipe(fd); // Fermeture du pipe
 *
 * @dependances: 
 *   - La fonction utilise 'close' de la bibliothèque standard pour fermer les descripteurs de fichier.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Le descripteur 'PIPE_READ_END' est-il ouvert ?
 *  /       \
 * OUI      NON
 *  |         |
 *  v         |
 * Fermer    Le descripteur 'PIPE_WRITE_END' est-il ouvert ?
 * 'PIPE_READ_END'  /       \
 *                  OUI      NON
 *                   |         |
 *                   v         |
 *                 Fermer    Retourner 1
 *                 'PIPE_WRITE_END'  
 *                   |
 *                   v
 *                 Retourner 1
 *                   |
 *                   v
 *                 Fin
 */
int	close_pipe(int *fd)
{
	if (fd[PIPE_READ_END] > 0)
		close(fd[PIPE_READ_END]);
	if (fd[PIPE_WRITE_END] > 0)
		close(fd[PIPE_WRITE_END]);
	return (1);
}

/**
 * @nom: close_fd
 * @brief: Ferme les descripteurs de fichier de tous les nœuds dans une liste de nœuds.
 *
 * @description:
 *   Parcourt une liste chaînée de nœuds (t_node), représentant des commandes dans un shell, et
 *   ferme les descripteurs de fichier d'entrée et de sortie pour chaque nœud. Cette fonction est 
 *   essentielle pour libérer les ressources système après l'exécution des commandes.
 *
 * @paramètres:
 *   Aucun. Utilise la variable globale 'g_all' pour accéder à la liste de nœuds.
 *
 * @pourquoi:
 *   - Gestion des Ressources Système : Assure que les descripteurs de fichier ouverts sont 
 *     correctement fermés, prévenant les fuites de ressources et les blocages potentiels.
 *   - Nettoyage Post-Exécution : Importante dans le cycle de vie d'un shell, pour réinitialiser 
 *     l'état après l'exécution des commandes, en particulier dans les pipelines et les redirections.
 *
 * @valeur_de_retour: 
 *   Retourne 1 comme indicateur de la fin de l'opération.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Les appels à 'close' peuvent échouer, mais ces échecs ne sont pas gérés par la fonction.
 *
 * @exemples_d'utilisation:
 *   // Après l'exécution des commandes dans un shell
 *   close_fd();
 *
 * @dependances: 
 *   - Utilise 'close' pour fermer les descripteurs de fichier.
 *   - Dépend de la structure globale 'g_all' pour accéder aux nœuds.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser avec la tête de la liste de nœuds (data->utils->head_node_lst)
 *     |
 *     v
 *   Parcourir chaque nœud dans la liste
 *     |
 *     v
 *   Le descripteur 'in' du nœud est-il ouvert ?
 *  /       \
 * OUI      NON
 *  |         |
 *  v         |
 * Fermer    Le descripteur 'out' du nœud est-il ouvert ?
 * 'in'     /       \
 *          OUI      NON
 *           |         |
 *           v         |
 *         Fermer    Passer au nœud suivant
 *         'out'     
 *           |
 *           v
 *         Passer au nœud suivant
 *           |
 *           v
 *         Fin de la liste ?
 *        /       \
 *      NON      OUI
 *       |         |
 *       v         v
 *    Continuer   Retourner 1
 *    la boucle  
 *       |
 *       v
 *     Fin
 */
int	close_fd(t_data *data)
{
	data->utils->node = data->utils->head_node_lst;
	while (data->utils->node)
	{
		if (data->utils->node->in > 1)
			close(data->utils->node->in);
		if (data->utils->node->out > 1)
			close(data->utils->node->out);
		data->utils->node = data->utils->node->next;
	}
	return (1);
}
