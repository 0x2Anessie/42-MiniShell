#include "../../include/minishell.h"

/**
 * @nom: get_env_var_expansion_length
 * @brief: Compare un mot avec une variable d'environnement et calcule la
 * longueur d'expansion.
 *
 * @description:
 * Cette fonction compare chaque caractère d'un mot donné avec une variable
 * d'environnement spécifiée et calcule la longueur supplémentaire nécessaire
 * pour l'expansion en cas de correspondance. Elle s'assure que le mot
 * correspond exactement à la variable avant le signe '='.
 *
 * @param word: char *word, le mot à comparer pour l'expansion.
 * @param length: int *length, pointeur vers la longueur totale calculée pour
 * l'expansion.
 * @param env_var: char *env_var, la variable d'environnement à comparer.
 * @param state: t_quote *state, pointeur vers l'état actuel des guillemets.
 *
 * @pourquoi:
 * - Correspondance Précise : Assure que seule une correspondance exacte entre
 * le mot et la variable d'environnement déclenchera l'expansion, évitant les
 * expansions incorrectes.
 * - Calcul Exact de la Longueur : Permet de déterminer avec précision
 * l'espace supplémentaire requis pour l'expansion, essentiel pour
 * l'allocation de mémoire et la formation correcte de la chaîne étendue.
 * - Prise en Compte des Caractères Spéciaux : Traite les cas où des caractères
 * spéciaux ou une autre variable d'environnement suivent immédiatement,
 * garantissant une expansion conforme aux règles du shell.
 *
 * @valeur_de_retour:
 * Retourne l'indice dans 'word' après avoir traité la partie actuelle pour
 * l'expansion.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Modifie la valeur de 'state->found' en cas de correspondance.
 * - Ajuste 'length' pour inclure la longueur des caractères d'expansion.
 *
 * @exemples_utilisation:
 * char *word = "$PATHextra";
 * char *env_var = "PATH=/usr/bin";
 * int length = 0;
 * t_quote state = {.singl_quot_status = 0, .doubl_quot_status = 0};
 * int index = get_env_var_expansion_length(word, &length, env_var, &state);
 *
 * @dependances: 
 *   - is_special_syntax_character pour vérifier les caractères spéciaux.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Comparaison de 'word' avec 'env_var'
 *     |
 *     v
 *   Correspondance complète trouvée ?
 *  /                  \
 * VRAI                FAUX
 *  |                   \
 *  |                    \
 *  v                     v
 * Vérifier              Retourner 'i'
 * la présence de '='
 *  /             \
 * VRAI           FAUX
 *  |              \
 *  |               \
 *  v                v
 * Mettre à jour       Retourner 'i'
 * 'length' et
 * 'state', retourner 'i'
 */
int	get_env_var_expansion_length(\
char *word, int *length, char *env_var, t_quote *state)
{
	int			i;
	int			y;

	i = ZERO_INIT;
	y = ZERO_INIT;
	while (word[i++] && env_var[y] \
	&& env_var[y] != '=' \
	&& word[i] == env_var[y])/*         ---> condition non intelligible --> fonction         */
		y++;
	if ((word[i] == '\0' \
	|| is_special_syntax_character(\
	word[i], state) || word[i] == '$') && env_var[y]
		&& y != 0 && env_var[y] == '=')/*         ---> condition non intelligible --> fonction         */
	{
		state->var_env_match_confirmed = 1;
		while (env_var[++y])/*         ---> condition non intelligible --> fonction         */
			(*length)++;
		return (i);
	}
	return (i);
}

/**
 * @nom: calculate_length_until_single_quote
 * @brief: Calcule la longueur d'expansion pour un mot dans des guillemets
 * simples.
 *
 * @description:
 * Parcourt un mot pour calculer sa longueur jusqu'à la prochaine occurrence
 * d'un guillemet simple, en supposant que le mot commence à l'intérieur de
 * guillemets simples. Cette fonction est utile pour traiter des chaînes entre
 * guillemets simples dans le contexte d'expansion de variables ou de
 * traitement de chaînes.
 *
 * @param word: char *word, le mot à évaluer pour l'expansion.
 * @param length: int *length, pointeur vers la longueur totale calculée pour
 * l'expansion.
 *
 * @pourquoi:
 * - Prise en Compte des Guillemets Simples : Les guillemets simples ont un
 * rôle spécifique dans le shell, empêchant l'expansion de variables. Cette
 * fonction calcule précisément la longueur des mots dans ces cas, assurant
 * une expansion correcte.
 * - Fiabilité dans le Traitement des Chaînes : Garantit que les chaînes
 * encadrées de guillemets simples sont traitées fidèlement, en respectant
 * les règles de citation du shell.
 *
 * @valeur_de_retour:
 * Retourne l'indice dans 'word' où le guillemet simple fermant est trouvé.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Modifie la valeur pointée par 'length' pour inclure la longueur du mot
 * jusqu'au guillemet simple fermant.
 *
 * @exemples_utilisation:
 * char *word = "'Hello world'";
 * int length = 0;
 * int index = calculate_length_until_single_quote(word, &length);
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser l'index 'index' à ZERO_INIT
 *     |
 *     v
 *   Parcourir 'word' jusqu'au prochain guillemet simple
 *     |
 *     v
 *   Incrémenter 'length' pour chaque caractère
 *     |
 *     v
 *   Guillemet simple fermant trouvé ?
 *  /       \
 * NON      OUI
 *  |         |
 *  v         v
 * Continuer  Retourner l'indice où le guillemet simple fermant est trouvé
 * la boucle
 *    |
 *    v
 *   Fin
 */
int	calculate_length_until_single_quote(char *word, int *length)
{
	int		index;

	index = ZERO_INIT;
	while (word[index] && word[index] != '\'')/*         ---> condition non intelligible --> fonction         */
	{
		(*length)++;
		index++;
	}
	return (index);
}

bool	is_char_matching_env_var(\
char *word, int word_index, char *env_var, int env_var_index)
{
    return (word[word_index] == env_var[env_var_index]);
}


int	calculate_length_for_env_expansion(\
char *word, char **env_var, t_quote *state, int *length)
{
	int			env_var_index;
	int			y;
	int			word_index;

	state->var_env_match_confirmed = ZERO_INIT;
	if (is_only_single_quote_on(state))
		return (calculate_length_until_single_quote(word, length));
	else
	{
		env_var_index = ZERO_INIT;
		while (env_var[env_var_index])/*         ---> condition non intelligible --> fonction         */
		{
			y = ZERO_INIT;
			word_index = 1;
			if (is_char_matching_env_var(word, word_index, env_var[env_var_index], y))
			{
				word_index = get_env_var_expansion_length(\
				word, length, env_var[env_var_index], state);
				if (state->var_env_match_confirmed == 1)/*         ---> condition non intelligible --> fonction         */
					return (word_index);
			}
			env_var_index++;
		}	
	}
	return (word_index);
}

/**
 * @nom: calculate_expanded_env_var_length
 * @brief: Calcule la longueur d'expansion pour un mot donné en fonction des
 * variables d'environnement.
 *
 * @description:
 * Évalue un mot et ses correspondances potentielles dans les variables
 * d'environnement pour déterminer la longueur totale de la chaîne après
 * expansion. Cette fonction prend en compte les guillemets et les variables
 * d'environnement pour ajuster la longueur d'expansion.
 *
 * @param word: char *word, pointeur vers le mot à évaluer pour l'expansion.
 * @param env_var: char **env_var, tableau de chaînes représentant les variables
 * d'environnement.
 * @param state: t_quote *state, pointeur vers l'état actuel des guillemets.
 * @param length: int *length, pointeur vers la longueur totale calculée.
 *
 * @pourquoi:
 * - Adaptation aux Variables d'Environnement : Permet de calculer correctement
 * la longueur nécessaire pour les mots qui contiennent des références aux
 * variables d'environnement, assurant une expansion précise et fidèle.
 * - Gestion des Guillemets : Prend en compte les règles de citation pour
 * l'expansion, évitant les expansions non désirées à l'intérieur des
 * guillemets simples.
 * - Flexibilité et Exactitude : Offre une méthode adaptable pour évaluer la
 * longueur d'expansion dans divers contextes et configurations de shell.
 *
 * @valeur_de_retour:
 * Retourne l'indice dans 'word' après avoir traité la partie actuelle pour
 * l'expansion.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Aucune erreur spécifique n'est définie pour cette fonction.
 * - Modifie la valeur pointée par 'length' pour refléter la longueur
 * d'expansion.
 *
 * @exemples_utilisation:
 * char *word = "$HOME/path/to/dir";
 * char *env[] = {"HOME=/home/user", NULL};
 * int length = 0;
 * t_quote state = {.singl_quot_status = 0, .doubl_quot_status = 0};
 * calculate_expanded_env_var_length(word, env, &state, &length);
 *
 * @dependances: 
 * - compare_and_expnd_length pour comparer et calculer la longueur
 * d'expansion.
 * - calculate_length_until_single_quote pour traiter les guillemets
 * simples.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier les guillemets dans 'state'
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Appeler       Parcourir 'env_var'
 * 'calculate_length_until_single_quote'   |
 * Retourner 'word_index'      v
 *                         Correspondance dans 'env_var' ?
 *                         /         \
 *                       VRAI       FAUX
 *                        |           \
 *                        |            \
 *                        v             v
 *                     Appeler       Incrémenter 'env_var_index'
 *                     'compare_and_expnd_length'  et continuer
 *                        |
 *                        v
 *                     'state->found' == 1 ?
 *                    /         \
 *                  VRAI       FAUX
 *                   |           \
 *                   |            \
 *                   v             v
 *                 Retourner    Continuer la boucle
 *                 'word_index'   sur 'env_var'
 *                               |
 *                               v
 *                             Retourner 'word_index'
 */
int	calculate_expanded_env_var_length(\
char *word, char **env_var, t_quote *state, int *length)
{
	state->var_env_match_confirmed = ZERO_INIT;
	if (is_only_single_quote_on(state))
		return (calculate_length_until_single_quote(word, length));
	else
	{
		return (calculate_length_for_env_expansion(\
		word, env_var, state, length));
	}
}
