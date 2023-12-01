
#include "../../include/minishell.h"

/**
 * @fonction: reset_quoting_state
 * @brief: Réinitialise l'état des guillemets dans une structure 't_quote'.
 * 
 * Cette fonction met à zéro les indicateurs d'état des guillemets simples et doubles
 * dans une structure 't_quote' fournie. Elle est utilisée pour réinitialiser l'état
 * avant de commencer le traitement d'une nouvelle chaîne ou d'un nouveau mot.
 * 
 * @param state: Pointeur sur la structure 't_quote' dont l'état doit être réinitialisé.
 * 
 * @erreur: Aucune erreur définie pour cette fonction.
 * 
 * @effet_de_bord: Réinitialise les champs 'is_dquote' et 'is_quote' de la structure pointée par 'state'.
 * 
 * @exemple: reset_quoting_state(state) réinitialise l'état des guillemets de 'state'.
 * 
 * @dependances: Aucune dépendance externe.
 */
void	reset_quoting_state(t_quote *state)
{
	state->is_dquote = ZERO_INIT;
	state->is_quote = ZERO_INIT;
}

/**
 * @nom: ft_len_nb
 * @brief: Calcule la longueur d'un nombre lorsqu'il est représenté en tant que chaîne de caractères.
 *
 * @description:
 *   Détermine le nombre de chiffres nécessaires pour représenter un entier 'n' sous forme de chaîne. 
 *   Cette fonction est particulièrement utile pour allouer la bonne quantité de mémoire lors de 
 *   la conversion d'entiers en chaînes de caractères, comme dans la fonction 'ft_itoa'.
 *
 * @param n: long n, l'entier dont la longueur est à calculer.
 *
 * @pourquoi:
 *   - Précision d'Allocation : Permet de déterminer la taille exacte nécessaire pour une chaîne de caractères 
 *     représentant un nombre, assurant une allocation de mémoire efficace.
 *   - Utilité dans la Conversion : Fournit une information essentielle pour la conversion de nombres en 
 *     chaînes, évitant les erreurs de dépassement de mémoire.
 *
 * @valeur_de_retour:
 *   Retourne le nombre de chiffres nécessaires pour représenter 'n' en tant que chaîne.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction ne modifie pas 'n' et n'a pas d'effets de bord.
 *
 * @exemples_d'utilisation:
 *   long nombre = 12345;
 *   int longueur = ft_len_nb(nombre); // longueur est 5
 *
 *   long nombre_zero = 0;
 *   int longueur_zero = ft_len_nb(nombre_zero); // longueur_zero est 1
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Initialiser 'length' à ZERO_INIT
 *     |
 *     v
 *   - Vérifier si 'n' est égal à zéro
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Retourner 1   - Parcourir 'n', en divisant par 10 à chaque itération
 *                   - Incrémenter 'length' à chaque étape
 *                   - Continuer jusqu'à ce que 'n' soit 0
 *                     |
 *                     v
 *                   - Retourner 'length'
 *                     |
 *                     v
 *                   Fin
 */
int	ft_len_nb(long n)
{
	int	length;

	length = ZERO_INIT;
	if (n == 0)
	{
		length = 1;
		return (length);
	}
	while (n != 0)
	{
		length++;
		n /= 10;
	}
	return (length);
}

/**
 * @nom: ft_is_neg
 * @brief: Traite les nombres négatifs pour la conversion en chaîne de caractères.
 *
 * @description:
 *   Convertit un nombre négatif 'n' en une chaîne de caractères, en ajoutant un signe moins 
 *   au début de la chaîne. La fonction alloue de la mémoire pour accueillir la représentation 
 *   textuelle du nombre, y compris le signe moins. Cette fonction est un complément à 'ft_itoa' 
 *   pour gérer spécifiquement les nombres négatifs.
 *
 * @param res: char *res, pointeur initial vers la chaîne de caractères résultante (non utilisé).
 * @param n: long n, le nombre négatif à convertir.
 * @param length: int length, la longueur calculée de la représentation numérique, sans compter le signe moins.
 *
 * @pourquoi:
 *   - Représentation Correcte des Négatifs : Assure que les nombres négatifs sont représentés 
 *     correctement avec un signe moins, conformément aux conventions numériques.
 *   - Modularité et Clarté : Sépare la logique de traitement des nombres négatifs de celle des 
 *     nombres positifs, rendant le code de 'ft_itoa' plus clair et modulaire.
 *
 * @valeur_de_retour:
 *   Retourne un pointeur vers la nouvelle chaîne de caractères représentant le nombre négatif. 
 *   En cas d'échec d'allocation, retourne NULL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si l'allocation de mémoire échoue, la fonction retourne NULL.
 *   - La chaîne retournée doit être libérée par l'appelant pour éviter les fuites de mémoire.
 *
 * @exemples_d'utilisation:
 *   long num_neg = -123;
 *   char *str_neg = ft_is_neg(NULL, num_neg, 3); // str_neg est "-123"
 *
 * @dependances: 
 *   - ft_malloc_with_tracking pour allouer la mémoire pour la chaîne de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Convertir 'n' en sa valeur absolue
 *     |
 *     v
 *   - Allouer de l'espace pour 'res' (incluant le signe moins)
 *     |
 *     v
 *   - Vérifier si l'allocation a réussi
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Placer le signe moins  - Retourner NULL
 *     au début de 'res'
 *     |
 *     v
 *   - Remplir 'res' avec les chiffres de 'n'
 *     |
 *     v
 *   - Terminer la chaîne avec '\0'
 *     |
 *     v
 *   - Retourner 'res'
 *     |
 *     v
 *   Fin
 */
char	*ft_is_neg(char *res, long n, int length)
{
	n = n * (-1);
	res = ft_malloc_with_tracking(sizeof(char) * length + sizeof('-') + sizeof('\0'));
	if (!res)
		return (NULL);
	res[0] = '-';
	res[length + FORWARD_OFFSET] = '\0';
	while (length >= 1)
	{
		res[length] = (n % 10) + '0';
		n /= 10;
		length--;
	}
	return (res);
}

/**
 * @nom: ft_itoa
 * @brief: Convertit un entier en une chaîne de caractères.
 *
 * @description:
 *   Convertit un entier 'n' en une chaîne de caractères représentative. 
 *   La fonction gère les valeurs négatives et alloue dynamiquement une nouvelle chaîne 
 *   pour contenir le résultat. Cette fonction est utile pour convertir des nombres en chaînes 
 *   pour l'affichage, la journalisation ou d'autres formes de traitement textuel.
 *
 * @param n: int n, l'entier à convertir en chaîne.
 *
 * @pourquoi:
 *   - Transformation de Données : Permet de convertir les données numériques en format textuel,
 *     facilitant leur manipulation dans divers contextes comme l'affichage ou la sauvegarde.
 *   - Compatibilité avec les Opérations de Chaînes : Rend possible l'utilisation de nombres 
 *     dans des contextes où les chaînes de caractères sont nécessaires ou plus pratiques.
 *
 * @valeur_de_retour:
 *   Retourne un pointeur vers la chaîne de caractères représentant le nombre 'n'.
 *   En cas d'échec d'allocation, retourne NULL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si l'allocation de mémoire échoue, la fonction retourne NULL.
 *   - La chaîne retournée doit être libérée par l'appelant pour éviter les fuites de mémoire.
 *
 * @exemples_d'utilisation:
 *   int num = 123;
 *   char *str = ft_itoa(num); // str est "123"
 *
 *   int num_neg = -456;
 *   char *str_neg = ft_itoa(num_neg); // str_neg est "-456"
 *
 * @dependances: 
 *   - ft_len_nb pour calculer la longueur du nombre.
 *   - ft_is_neg pour gérer les nombres négatifs.
 *   - ft_malloc_with_tracking pour l'allocation de la chaîne.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Conversion de 'n' en long et initialisation de 'res'
 *     |
 *     v
 *   - Calcul de la longueur nécessaire pour 'res'
 *     |
 *     v
 *   - Vérifier si 'nb' est négatif
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Gérer le cas négatif  - Allouer de l'espace pour 'res'
 *     avec ft_is_neg          |
 *     et retourner 'res'      |
 *                             v
 *   - Remplir 'res' avec    - Vérifier si l'allocation a réussi
 *     les chiffres de 'nb'  /       \
 *                           OUI      NON
 *                            |         |
 *                            v         v
 *   - Terminer la chaîne    - Retourner NULL
 *     avec '\0'
 *     |
 *     v
 *   - Retourner 'res'
 *     |
 *     v
 *   Fin
 */
char	*ft_itoa(int n)
{
	long	nb;
	int		index;
	char	*res;

	nb = (long)n;
	res = ZERO_INIT;
	index = ft_len_nb(nb);
	if (nb < 0)
	{
		res = ft_is_neg(res, nb, index);
		return (res);
	}
	res = ft_malloc_with_tracking(sizeof(char) * (index + sizeof('\0')));
	res[index] = '\0';
	if (!res)
		return (NULL);
	index--;
	while (index >= 0)
	{
		res[index] = (nb % 10) + '0';
		nb /= 10;
		index--;
	}
	return (res);
}

/**
 * @nom: ft_isdigit
 * @brief: Vérifie si un caractère est un chiffre décimal.
 *
 * @description:
 *   La fonction 'ft_isdigit' détermine si le caractère donné 'c' est un chiffre décimal (0 à 9). 
 *   Cette fonction est essentielle pour les opérations de traitement de texte où il est nécessaire 
 *   de distinguer les chiffres des autres caractères.
 *
 * @param c: char c, le caractère à vérifier.
 *
 * @pourquoi:
 *   - Validation de Chiffres : Permet de vérifier si un caractère fait partie des chiffres décimaux, 
 *     ce qui est utile dans les contextes de validation de données numériques, de parsing ou 
 *     de manipulation de chaînes de caractères.
 *   - Universel et Réutilisable : Fournit une implémentation standardisée et facilement réutilisable 
 *     pour vérifier les caractères numériques, réduisant ainsi la redondance du code.
 *
 * @valeur_de_retour:
 *   Retourne 1 (vrai) si 'c' est un chiffre, sinon 0 (faux).
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction est sans effet de bord et ne modifie pas 'c'.
 *
 * @exemples_d'utilisation:
 *   char c1 = '5';
 *   int result1 = ft_isdigit(c1); // Retourne 1 (vrai)
 *   
 *   char c2 = 'a';
 *   int result2 = ft_isdigit(c2); // Retourne 0 (faux)
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Vérifier si 'c' est compris entre '0' et '9'
 *   /                   \
 *  OUI                  NON
 *   |                    |
 *   v                    v
 *   - Retourner 1        - Retourner 0
 *         \                     /
 *          \___________________/
 *                   |
 *                   v
 *                  Fin
 */
int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}
