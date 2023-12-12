
#include "../../include/minishell.h"

/**
 * @nom: env_size
 *
 * @description:
 *   Calcule la taille d'un tableau de chaînes de caractères représentant les 
 *   variables d'environnement. Cette fonction parcourt le tableau jusqu'à un 
 *   élément NULL, indiquant la fin du tableau, et compte le nombre d'éléments.
 *
 * @pourquoi:
 *   - Détermination de la taille de l'environnement : Savoir combien de variables 
 *     d'environnement sont présentes est essentiel pour diverses opérations, 
 *     telles que l'initialisation de listes chaînées ou les boucles de traitement.
 *   - Base pour les opérations dynamiques : En connaissant la taille de l'environnement, 
 *     les fonctions peuvent allouer correctement la mémoire et éviter les 
 *     dépassements de tableau, ce qui est crucial pour la stabilité et la sécurité 
 *     du programme.
 *   - Compatibilité avec les structures de données : Cette taille est utilisée pour 
 *     créer des structures qui représentent l'environnement de manière plus 
 *     structurée et manipulable, comme des listes chaînées.
 *
 * @paramètres:
 *   - env: Un tableau de pointeurs vers des chaînes de caractères, chacune 
 *     représentant une variable d'environnement.
 *
 * @fonctionnement:
 *   - Initialise un compteur (index) à zéro.
 *   - Parcourt le tableau 'env' jusqu'à un élément NULL.
 *   - Incrémente le compteur pour chaque élément non-NULL.
 *   - Retourne la valeur du compteur, représentant le nombre d'éléments dans 'env'.
 *
 * @valeur_de_retour:
 *   - Retourne le nombre d'éléments dans le tableau 'env' avant le premier élément NULL.
 *
 * @erreurs_et_effets_de_bord:
 *   - Si 'env' est NULL, la fonction retourne 0. La fonction suppose que 'env' est 
 *     correctement initialisé et terminé par un élément NULL.
 *
 * @exemples_d'utilisation:
 *   - env_size(environ) pour obtenir le nombre de variables d'environnement 
 *     dans le tableau 'environ'.
 *
 * @dépendances:
 *   Aucune dépendance externe.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser index à ZERO_INIT
 *     |
 *     v
 *   Boucle tant que env[index] n'est pas NULL
 *     |       |
 *     |       |
 *     |       v
 *     |     Incrémenter index
 *     |       |
 *     |       v
 *     |     Retour à la boucle
 *     |
 *     v
 *   Retourner index
 */
int	env_size(char **env)
{
	int	index;

	index = ZERO_INIT;
	while (env[index])
		index++;
	return (index);
}

/**
 * @nom: create_and_init_env_var_node
 *
 * @description:
 *   Crée un nouvel élément de type t_env pour une variable d'environnement. Cette 
 *   fonction alloue de la mémoire pour un nouvel élément, copie la variable 
 *   d'environnement fournie dans 'content' et initialise le pointeur 'next' à NULL.
 *
 * @pourquoi:
 *   - Création d'éléments pour la liste chaînée : La création d'éléments individuels 
 *     est une étape nécessaire pour construire une liste chaînée représentant les 
 *     variables d'environnement. Cela permet de gérer les variables d'environnement 
 *     de manière structurée et dynamique.
 *   - Isolation et gestion des variables : Chaque variable d'environnement est 
 *     encapsulée dans sa propre structure, facilitant les opérations telles que 
 *     l'ajout, la suppression ou la modification de variables individuelles sans 
 *     affecter les autres.
 *   - Allocation mémoire contrôlée : L'utilisation de 'ft_malloc_with_tracking' 
 *     pour l'allocation permet un suivi et une gestion plus précise de la mémoire, 
 *     notamment pour éviter les fuites de mémoire dans des programmes de longue durée.
 *
 * @paramètres:
 *   - env: Une chaîne de caractères représentant la variable d'environnement à 
 *     encapsuler dans la nouvelle structure t_env.
 *
 * @fonctionnement:
 *   - Alloue de la mémoire pour un nouvel élément de type t_env.
 *   - En cas d'échec de l'allocation, retourne NULL.
 *   - Copie la chaîne 'env' dans 'content' du nouvel élément.
 *   - Initialise 'next' à NULL pour marquer la fin de la liste chaînée.
 *   - Retourne un pointeur sur le nouvel élément.
 *
 * @valeur_de_retour:
 *   - Retourne un pointeur sur le nouvel élément de type t_env.
 *   - Retourne NULL en cas d'échec de l'allocation mémoire.
 *
 * @erreurs_et_effets_de_bord:
 *   - Si l'allocation mémoire échoue, la fonction retourne NULL sans créer 
 *     de nouvel élément.
 *
 * @exemples_d'utilisation:
 *   - create_and_init_env_var_node("PATH=/usr/bin") pour créer un élément contenant la variable 
 *     d'environnement PATH.
 *
 * @dépendances:
 *   - ft_malloc_with_tracking: Alloue de la mémoire avec suivi.
 *   - ft_strdup: Copie une chaîne de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Allouer de la mémoire pour 'new'
 *     |
 *     v
 *   'new' est NULL ?
 *    /       \
 *  VRAI     FAUX
 *    |         |
 *    |         |
 *    v         v
 *   Retourner  Copier 'env' dans 'new->content'
 *   NULL       Initialiser 'new->next' à NULL
 *              |
 *              v
 *           Retourner 'new'
 */
t_env	*create_and_init_env_var_node(t_data *data, char *env)
{
	t_env	*new;

	new = ft_malloc_with_tracking(data, sizeof(t_env));
	if (!new)
		return (NULL);
	new->content = ft_strdup(data, env);
	new->next = NULL;
	return (new);
}

/**
 * @nom: ft_env_lst_add_to_end
 *
 * @description:
 *   Ajoute un nouvel élément à la fin d'une liste chaînée de type t_env. Cette 
 *   fonction parcourt la liste jusqu'à atteindre le dernier élément, puis y attache 
 *   le nouvel élément, permettant l'extension dynamique de la liste.
 *
 * @pourquoi:
 *   - Extension dynamique de la liste : La capacité d'ajouter de nouveaux éléments 
 *     à la fin de la liste permet une gestion flexible des variables d'environnement, 
 *     ce qui est crucial dans les contextes où ces variables peuvent changer ou 
 *     être étendues au fil de l'exécution.
 *   - Maintien de l'ordre des éléments : Ajouter de nouveaux éléments à la fin 
 *     préserve l'ordre initial des variables d'environnement, ce qui peut être 
 *     important pour certaines applications qui dépendent de l'ordre de ces variables.
 *   - Simplicité et efficacité : Cette méthode d'ajout est simple et efficace, 
 *     évitant la complexité et le coût potentiel de réorganiser ou de réallouer 
 *     des structures de données plus grandes.
 *
 * @paramètres:
 *   - lst: Un pointeur sur le premier élément de la liste chaînée de type t_env.
 *   - new: Un pointeur sur l'élément de type t_env à ajouter à la fin de la liste.
 *
 * @fonctionnement:
 *   - Initialise un pointeur temporaire (tmp) pour parcourir la liste.
 *   - Parcourt la liste jusqu'à atteindre le dernier élément (tmp->next est NULL).
 *   - Lie le dernier élément de la liste (tmp) au nouvel élément (new).
 *
 * @valeur_de_retour:
 *   Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 *   - Si 'lst' est NULL, la fonction ne fait rien. Il incombe à l'appelant de 
 *     s'assurer que la liste n'est pas NULL avant d'appeler cette fonction.
 *
 * @exemples_d'utilisation:
 *   - ft_env_lst_add_to_end(env_list, new_env_var) pour ajouter une nouvelle 
 *     variable d'environnement à une liste existante.
 *
 * @dépendances:
 *   Aucune dépendance spécifique à d'autres fonctions ou structures externes.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser tmp à lst
 *     |
 *     v
 *   Boucle (tmp->next n'est pas NULL) ?
 *    /        \
 *  VRAI      FAUX
 *    |         |
 *    |         |
 *    v         v
 *   Passer à  Lie tmp->next à new
 *   tmp->next 
 *     |
 *     v
 *   Retour à la boucle
 *     |
 *     v
 *   Fin
 */
void	ft_env_lst_add_to_end(t_env *lst, t_env *new)
{
	t_env	*tmp;

	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/**
 * @nom: create_env_list_from_array
 *
 * @description:
 *   Crée une liste chaînée à partir d'un tableau d'environnements. Cette fonction 
 *   convertit les variables d'environnement fournies sous forme de tableau de 
 *   chaînes en une liste chaînée de type t_env, facilitant leur manipulation et 
 *   leur accès au sein du programme.
 *
 * @pourquoi:
 *   - Structuration des données d'environnement : La conversion d'un tableau en 
 *     liste chaînée permet une gestion plus flexible et structurée des variables 
 *     d'environnement, en particulier pour les opérations d'ajout, de suppression 
 *     ou de recherche.
 *   - Facilité de manipulation : Les listes chaînées sont souvent plus pratiques 
 *     pour manipuler des ensembles de données dynamiques, comme l'est l'environnement 
 *     dans un shell ou une application similaire.
 *   - Prise en charge des environnements vides : En initialisant une liste avec la 
 *     variable PWD si l'environnement est vide, la fonction assure que le programme 
 *     a toujours accès à au moins une variable d'environnement essentielle.
 *
 * @paramètres:
 *   - env: Un tableau de chaînes de caractères représentant les variables 
 *     d'environnement.
 *
 * @fonctionnement:
 *   - Calcule le nombre de variables d'environnement dans le tableau 'env'.
 *   - Si le tableau est vide, initialise la liste avec la variable PWD.
 *   - Sinon, parcourt chaque variable d'environnement, crée un nouvel élément 
 *     de type t_env et l'ajoute à la fin de la liste chaînée.
 *   - Retourne la liste chaînée de variables d'environnement.
 *
 * @valeur_de_retour:
 *   - Retourne un pointeur sur le début de la liste chaînée de type t_env 
 *     contenant les variables d'environnement.
 *   - Retourne NULL si le tableau d'environnement est vide et ne peut pas être 
 *     initialisé.
 *
 * @erreurs_et_effets_de_bord:
 *   - Si l'allocation mémoire pour un nouvel élément échoue, la fonction continue 
 *     avec les éléments suivants, potentiellement aboutissant à une liste 
 *     incomplète.
 *
 * @exemples_d'utilisation:
 *   - create_env_list_from_array(envp) dans un programme où envp est le tableau de variables 
 *     d'environnement passé au main.
 *
 * @dépendances:
 *   - env_size: Calcule la taille du tableau d'environnement.
 *   - init_env_list_with_pwd_if_empty: Initialise la liste avec PWD si vide.
 *   - create_and_init_env_var_node: Crée un nouvel élément de type t_env.
 *   - ft_env_lst_add_to_end: Ajoute un élément à la fin de la liste chaînée.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Calculer env_nb (taille de l'environnement)
 *     |
 *     v
 *   env_nb est 0 ?
 *    /       \
 *  VRAI     FAUX
 *    |         \
 *    |          \
 *    v           v
 *   Initialiser  Initialiser i à -1, new et final à NULL
 *   avec PWD     |
 *    |           v
 *    |         Boucle (i++ < env_nb) ?
 *    |          /       \
 *    |        VRAI     FAUX
 *    |         |         |
 *    |         |         |
 *    |         v         v
 *    |        Créer    Retourner final
 *    |        un nouvel 
 *    |        élément 'new'
 *    |         |
 *    |         v
 *    |       final est NULL ?
 *    |        /        \
 *    |      VRAI      FAUX
 *    |       |          |
 *    |       |          |
 *    |       v          v
 *    |     Assigner    Ajouter 'new'
 *    |     'new' à     à la fin de
 *    |     'final'     'final'
 *    |       |
 *    |       v
 *    |     Retour à la boucle
 *    |
 *    v
 *   Retourner final
 */
t_env	*create_env_list_from_array(t_data *data, char **env)
{
	int		env_nb;
	int		i;
	t_env	*new;
	t_env	*final;

	i = -1;
	new = NULL;
	final = NULL;
	env_nb = env_size(env);
	if (env_nb == 0)
	{
		final = init_env_list_with_pwd_if_empty(data, final);
		return (final);
	}
	while (++i < env_nb)
	{
		new = create_and_init_env_var_node(data, env[i]);
		if (!final)
			final = new;
		else
			ft_env_lst_add_to_end(final, new);
	}
	return (final);
}
