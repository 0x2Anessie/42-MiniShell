#include "../../include/minishell.h"

bool	is_sorting_linked_list_env_var_unnecessary(\
t_export **head_of_linked_list_env_var)
{
	return (!head_of_linked_list_env_var \
	|| !(*head_of_linked_list_env_var) \
	|| !((*head_of_linked_list_env_var)->next));
}

bool	is_current_value_greater_than_next(\
char *current_value, char *next_value)
{
	return (ft_strcmp(current_value, next_value) > 0);
}

 
/**
 * @nom: sort_linked_list_env_var
 *
 * @description: Trie une liste de structures t_export représentant des
 * variables d'environnement, en utilisant un tri à bulles. Ce tri est
 * fondamental pour présenter les variables d'environnement dans un ordre
 * cohérent et structuré, facilitant leur gestion et leur manipulation.
 *
 * @parametres:
 *   - head_export: t_export **head_export, double pointeur vers la tête de
 * la liste des structures t_export à trier.
 *
 * @fonctionnement:
 *   La fonction commence par vérifier si la liste est valide et contient plus
 * d'un élément. Elle utilise ensuite une boucle pour parcourir la liste et
 * effectuer des échanges de valeurs entre les nœuds adjacents si nécessaire,
 * en se basant sur le résultat de la comparaison avec `ft_strcmp`. Le
 * processus se répète jusqu'à ce qu'aucun échange ne soit nécessaire dans un
 * passage complet, indiquant que la liste est triée.
 *
 *   Pourquoi utiliser un tri à bulles ?
 *   - Simplicité : Le tri à bulles est simple à implémenter, en particulier
 * pour des listes de petite à moyenne taille.
 *   - Adéquation avec les listes liées : Ce type de tri est adapté pour les
 * listes liées, car il ne nécessite pas d'accès aléatoire.
 *   - Besoin de tri : Trier les variables d'environnement rend l'affichage et
 * la manipulation plus intuitifs et organisés.
 *
 * @valeur_retour: 
 *   Aucune (void). La fonction modifie la liste en place.
 *
 * @erreurs: 
 *   - Aucune erreur spécifique n'est générée par cette fonction. 
 *
 * @exemple_utilisation:
 *   t_export *head_export = create_export_list();
 *   sort_linked_list_env_var(&head_export);
 *
 * @dependances: 
 *   - ft_strcmp pour comparer les chaînes de caractères des valeurs des
 * nœuds.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier si la liste est non NULL et contient au moins deux éléments
 *     |
 *     v
 *   Initialiser la variable 'swapped' à 1
 *     |
 *     v
 *   Entrer dans la boucle tant que 'swapped' est vrai
 *     |
 *     v
 *   Initialiser 'swapped' à 0 pour la nouvelle itération
 *     |
 *     v
 *   Parcourir la liste avec un pointeur 'current'
 *     |
 *     v
 *   Comparer 'current->value' avec 'current->next->value' (ft_strcmp)
 *     |
 *     v
 *   Si 'ft_strcmp' indique un ordre incorrect
 *   |  \
 *   |   v
 *   |  Échanger les valeurs de 'current' et 'current->next'
 *   |   |
 *   |   v
 *   |  Mettre 'swapped' à 1
 *   |   |
 *   |   v
 *   v  Passer à 'current->next'
 * Continuer jusqu'à la fin de la liste
 *     |
 *     v
 *   Si 'swapped' est 0 (aucun échange), la liste est triée
 *     |
 *     v
 *   Fin
 */
void	sort_linked_list_env_var(t_export **head_of_linked_list_env_var)
{
	int			swapped;
	t_export	*current;
	char		*temp;

	if (is_sorting_linked_list_env_var_unnecessary(\
	head_of_linked_list_env_var))
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *head_of_linked_list_env_var;
		while (!current->next)
		{
			if (is_current_value_greater_than_next(\
			current->value, current->next->value))
			{
				temp = current->value;
				current->value = current->next->value;
				current->next->value = temp;
				swapped = 1;
			}
			current = current->next;
		}
	}
}

/**
 * @nom: ft_new_variable_to_linked_list_env_var
 *
 * @description: Crée un nouveau nœud pour une liste d'exportation des
 * variables d'environnement.Cette fonction est essentielle pour construire
 * une liste structurée des variables d'environnement, facilitant ainsi leur
 * gestion et leur accès.
 *
 * @parametres:
 *   - new: t_export *new, pointeur vers le nouveau nœud à créer.
 *
 * @fonctionnement:
 * Alloue de la mémoire pour un nouveau nœud d'exportation en utilisant
 * `ft_malloc_with_tracking`, qui assure un suivi de l'allocation mémoire.
 * Utilise `ft_strdup` pour dupliquer la valeur de la variable
 * d'environnement actuelle dans le nœud. Initialise le champ `next` du nœud
 * à NULL.
 *
 *   Pourquoi créer un nouveau nœud d'exportation ?
 *   - Organisation structurée : Facilite la gestion des variables
 * d'environnement en les organisant dans une structure de liste liée.
 *   - Suivi de l'allocation mémoire : Permet de suivre les allocations
 * mémoire, crucial pour la gestion des ressources et la prévention des
 * fuites de mémoire.
 *   - Préparation pour l'exportation : Crée une représentation adéquate des
 * variables d'environnement pour leur manipulation ultérieure, notamment pour
 * l'exportation ou l'affichage.
 *
 * @valeur_retour: 
 * Renvoie un pointeur vers le nouveau nœud créé. Renvoie NULL en cas d'échec
 * d'allocation de mémoire.
 *
 * @erreurs: 
 *   - Renvoie NULL si l'allocation de mémoire échoue à n'importe quel stade.
 *
 * @exemple_utilisation:
 *   t_export *new_node = ft_new_variable_to_linked_list_env_var(new_node);
 *
 * @dependances: 
 *   - ft_malloc_with_tracking pour l'allocation mémoire avec suivi.
 *   - ft_strdup pour dupliquer la valeur de la variable d'environnement.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Allouer la mémoire pour le nouveau nœud (ft_malloc_with_tracking)
 *     |
 *     v
 *   Allocation réussie ?
 *  /       \
 * NON      OUI
 *  |        |
 *  v        v
 * Retourner NULL   Appeler malloc dans ft_malloc_with_tracking
 *                  |
 *                  v
 *                Appeler add_to_trash_memory
 *                  |
 *                  v
 *                Dupliquer la valeur de la variable (ft_strdup)
 *                  |
 *                  v
 *                Allocation pour la duplication réussie ?
 *               /       \
 *             NON      OUI
 *              |        |
 *              v        v
 *          Retourner NULL   Initialiser le champ next du nœud à NULL
 *                           |
 *                           v
 *                         Retourner le pointeur vers le nouveau nœud
 *                           |
 *                           v
 *                          Fin
 */
t_export	*ft_new_variable_to_linked_list_env_var(\
t_data *data, t_export *new)
{
	new = NULL;
	new = (t_export *)ft_malloc_with_tracking(data, sizeof(t_export));
	if (!new)
		return (NULL);
	new->value = ft_strdup(\
	data, data->utils->linked_list_full_env_var_copy_alpha->content);
	new->next = NULL;
	return (new);
}

/**
 * @nom: ft_buil_sorted_linked_list_env_var
 *
 * @description: Construit une liste triée des variables d'environnement pour
 * l'exportation. Cette fonction joue un rôle crucial dans la gestion ordonnée
 * des variables d'environnement, facilitant leur visualisation et leur
 * manipulation.
 *
 * @parametres:
 *   - head_of_linked_list_env_var: t_export *head_of_linked_list_env_var,
 * pointeur initial vers la liste d'exportation.
 *
 * @fonctionnement:
 * Initialise un nouveau pointeur de liste et parcourt la liste des
 * variables d'environnement. Pour chaque variable, crée un nouveau nœud avec
 * `ft_new_variable_to_linked_list_env_var` et l'ajoute à la liste. Après avoir
 * construit la liste complète, trie les éléments en utilisant
 * `sort_linked_list_env_var`, qui repose sur `ft_strcmp` pour comparer les
 * valeurs des nœuds. Ce processus garantit que les variables d'environnement
 * sont listées dans un ordre alphabétique pour une meilleure lisibilité et
 * cohérence.
 *
 *   Pourquoi construire et trier une liste d'exportation ?
 *   - Visualisation et gestion ordonnées : Facilite la visualisation et la
 * gestion des variables d'environnement, en les triant dans un ordre
 * compréhensible.
 *   - Préparation pour l'exportation : Rend l'exportation des variables plus
 * intuitive, en particulier dans des contextes où l'ordre des variables peut
 * être important.
 *
 * @valeur_retour: 
 *   Renvoie un pointeur vers le début de la liste triée des variables
 * d'environnement.
 *
 * @erreurs: 
 *   - Renvoie NULL en cas d'échec de l'allocation de mémoire.
 *
 * @exemple_utilisation:
 *   t_export *head_of_linked_list_env_var = NULL;
 *   head_of_linked_list_env_var = 
 * 			ft_buil_sorted_linked_list_env_var(head_of_linked_list_env_var);
 *
 * @dependances: 
 *   - ft_new_variable_to_linked_list_env_var pour créer de nouveaux nœuds de
 * liste.
 *   - sort_linked_list_env_var pour trier la liste.
 *   - ft_strcmp pour comparer les chaînes de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser un nouveau pointeur de liste
 *     |
 *     v
 *   Parcourir la liste des variables d'environnement
 *     |
 *     v
 *   Créer un nouveau nœud avec ft_new_variable_to_linked_list_env_var
 *     |
 *     v
 *   Ajouter le nouveau nœud à la liste d'exportation
 *     |
 *     v
 *   Toute la liste est-elle parcourue ?
 *  /       \
 * NON      OUI
 *  |        |
 *  v        v
 * Continuer Ajouter le nœud à la liste
 * la création  |
 * de nœuds    v
 *            Trier la liste avec sort_linked_list_env_var
 *             |
 *             v
 *           Retourner la liste d'exportation triée
 *             |
 *             v
 *            Fin
 */
t_export	*ft_buil_sorted_linked_list_env_var(\
t_data *data, t_export *head_of_linked_list_env_var)
{
	t_export	*new;
	t_export	*current_new;

	current_new = NULL;
	while (data->utils->linked_list_full_env_var_copy_alpha)
	{
		new = ft_new_variable_to_linked_list_env_var(data, new);
		if (!head_of_linked_list_env_var)
		{
			head_of_linked_list_env_var = new;
			current_new = head_of_linked_list_env_var;
		}
		else
		{
			current_new->next = new;
			current_new = current_new->next;
		}
		data->utils->linked_list_full_env_var_copy_alpha = data->utils->linked_list_full_env_var_copy_alpha->next;
	}
	sort_linked_list_env_var(&head_of_linked_list_env_var);
	data->utils->linked_list_full_env_var_copy_alpha = data->utils->head_env_lst;
	return (head_of_linked_list_env_var);
}
