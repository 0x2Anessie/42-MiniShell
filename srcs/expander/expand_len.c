/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 12:56:35 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/26 22:39:21 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @nom: singl_quot_expns_length
 * @brief: Calcule la longueur d'expansion pour un mot dans des guillemets simples.
 *
 * @description:
 *   Parcourt un mot pour calculer sa longueur jusqu'à la prochaine occurrence d'un 
 *   guillemet simple, en supposant que le mot commence à l'intérieur de guillemets simples. 
 *   Cette fonction est utile pour traiter des chaînes entre guillemets simples dans le contexte
 *   d'expansion de variables ou de traitement de chaînes.
 *
 * @param word: char *word, le mot à évaluer pour l'expansion.
 * @param length: int *length, pointeur vers la longueur totale calculée pour l'expansion.
 *
 * @pourquoi:
 *   - Prise en Compte des Guillemets Simples : Les guillemets simples ont un rôle spécifique dans 
 *     le shell, empêchant l'expansion de variables. Cette fonction calcule précisément la longueur 
 *     des mots dans ces cas, assurant une expansion correcte.
 *   - Fiabilité dans le Traitement des Chaînes : Garantit que les chaînes encadrées de guillemets 
 *     simples sont traitées fidèlement, en respectant les règles de citation du shell.
 *
 * @valeur_de_retour:
 *   Retourne l'indice dans 'word' où le guillemet simple fermant est trouvé.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Modifie la valeur pointée par 'length' pour inclure la longueur du mot jusqu'au guillemet simple fermant.
 *
 * @exemples_d'utilisation:
 *   char *word = "'Hello world'";
 *   int length = 0;
 *   int index = singl_quot_expns_length(word, &length);
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
 *  Fin
 */

static int	singl_quot_expns_length(char *word, int *length)
{
	int		index;

	index = ZERO_INIT;
	while (word[index] && word[index] != '\'')
	{
		(*length)++;
		index++;
	}
	return (index);
}

/**
 * @nom: compare_and_expnd_length
 * @brief: Compare un mot avec une variable d'environnement et calcule la longueur d'expansion.
 *
 * @description:
 *   Cette fonction compare chaque caractère d'un mot donné avec une variable d'environnement spécifiée
 *   et calcule la longueur supplémentaire nécessaire pour l'expansion en cas de correspondance.
 *   Elle s'assure que le mot correspond exactement à la variable avant le signe '='.
 *
 * @param word: char *word, le mot à comparer pour l'expansion.
 * @param length: int *length, pointeur vers la longueur totale calculée pour l'expansion.
 * @param env_var: char *env_var, la variable d'environnement à comparer.
 * @param state: t_quote *state, pointeur vers l'état actuel des guillemets.
 *
 * @pourquoi:
 *   - Correspondance Précise : Assure que seule une correspondance exacte entre le mot et la variable
 *     d'environnement déclenchera l'expansion, évitant les expansions incorrectes.
 *   - Calcul Exact de la Longueur : Permet de déterminer avec précision l'espace supplémentaire requis
 *     pour l'expansion, essentiel pour l'allocation de mémoire et la formation correcte de la chaîne étendue.
 *   - Prise en Compte des Caractères Spéciaux : Traite les cas où des caractères spéciaux ou une autre 
 *     variable d'environnement suivent immédiatement, garantissant une expansion conforme aux règles du shell.
 *
 * @valeur_de_retour:
 *   Retourne l'indice dans 'word' après avoir traité la partie actuelle pour l'expansion.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Modifie la valeur de 'state->found' en cas de correspondance.
 *   - Ajuste 'length' pour inclure la longueur des caractères d'expansion.
 *
 * @exemples_d'utilisation:
 *   char *word = "$PATHextra";
 *   char *env_var = "PATH=/usr/bin";
 *   int length = 0;
 *   t_quote state = {.is_quote = 0, .is_dquote = 0};
 *   int index = compare_and_expnd_length(word, &length, env_var, &state);
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
static int	compare_and_expnd_length(char *word, int *length, char *env_var, t_quote *state)
{
	int			i;
	int			y;

	i = ZERO_INIT;
	y = ZERO_INIT;
	while (word[i++] && env_var[y] && env_var[y] != '=' && word[i] == env_var[y])
		y++;
	if ((word[i] == '\0' || is_special_syntax_character(word[i], state) || word[i] == '$') && env_var[y]
		&& y != 0 && env_var[y] == '=')
	{
		state->found = 1;
		while (env_var[++y])
			(*length)++;
		return (i);
	}
	return (i);
}

/**
 * @nom: search_in_env_len
 * @brief: Calcule la longueur d'expansion pour un mot donné en fonction des variables d'environnement.
 *
 * @description:
 *   Évalue un mot et ses correspondances potentielles dans les variables d'environnement pour 
 *   déterminer la longueur totale de la chaîne après expansion. Cette fonction prend en compte 
 *   les guillemets et les variables d'environnement pour ajuster la longueur d'expansion.
 *
 * @param word: char *word, pointeur vers le mot à évaluer pour l'expansion.
 * @param env_var: char **env_var, tableau de chaînes représentant les variables d'environnement.
 * @param state: t_quote *state, pointeur vers l'état actuel des guillemets.
 * @param length: int *length, pointeur vers la longueur totale calculée.
 *
 * @pourquoi:
 *   - Adaptation aux Variables d'Environnement : Permet de calculer correctement la longueur nécessaire 
 *     pour les mots qui contiennent des références aux variables d'environnement, assurant une expansion 
 *     précise et fidèle.
 *   - Gestion des Guillemets : Prend en compte les règles de citation pour l'expansion, évitant les 
 *     expansions non désirées à l'intérieur des guillemets simples.
 *   - Flexibilité et Exactitude : Offre une méthode adaptable pour évaluer la longueur d'expansion 
 *     dans divers contextes et configurations de shell.
 *
 * @valeur_de_retour:
 *   Retourne l'indice dans 'word' après avoir traité la partie actuelle pour l'expansion.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Aucune erreur spécifique n'est définie pour cette fonction.
 *   - Modifie la valeur pointée par 'length' pour refléter la longueur d'expansion.
 *
 * @exemples_d'utilisation:
 *   char *word = "$HOME/path/to/dir";
 *   char *env[] = {"HOME=/home/user", NULL};
 *   int length = 0;
 *   t_quote state = {.is_quote = 0, .is_dquote = 0};
 *   search_in_env_len(word, env, &state, &length);
 *
 * @dependances: 
 *   - compare_and_expnd_length pour comparer et calculer la longueur d'expansion.
 *   - singl_quot_expns_length pour traiter les guillemets simples.
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
 * 'singl_quot_expns_length'   |
 * Retourner 'word_Index'      v
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
 *                 'word_Index'   sur 'env_var'
 *                               |
 *                               v
 *                             Retourner 'word_Index'
 */
int	search_in_env_len(char *word, char **env_var, t_quote *state, int *length)
{
	int			env_var_index;
	int			y;
	int			word_Index;

	state->found = ZERO_INIT;
	if (state->is_quote == 1 && state->is_dquote == 0)
		return (singl_quot_expns_length(word, length));
	else
	{
		env_var_index = ZERO_INIT;
		while (env_var[env_var_index])
		{
			y = ZERO_INIT;
			word_Index = 1;
			if (word[word_Index] == env_var[env_var_index][y])
			{
				word_Index = compare_and_expnd_length(word, length, env_var[env_var_index], state);
				if (state->found == 1)
					return (word_Index);
			}
			env_var_index++;
		}	
	}
	return (word_Index);
}

/**
 * @nom: sum_expansion_length
 * @brief: Calcule la longueur totale de la chaîne après expansion.
 * 
 * @description:
 *   Évalue chaque caractère d'un mot dans une structure lexer et calcule la longueur totale 
 *   de la chaîne après avoir pris en compte les expansions nécessaires, les caractères 
 *   spéciaux et les différentes règles d'expansion.
 * 
 * @param expnd: Pointeur sur un lexer (t_lexer) à traiter pour l'expansion.
 * @param state: État actuel des guillemets (t_quote).
 * @param nv: Tableau de chaînes de caractères représentant les variables d'environnement.
 * @param exp: Structure représentant l'état de l'expansion (t_expand).
 * 
 * @erreur: Aucune erreur spécifique n'est définie pour cette fonction.
 * 
 * @effet_de_bord: Modifie la longueur totale dans 'exp->len' et le contenu de 'exp->str'.
 * 
 * @exemple: sum_expansion_length(&lexer, state, env, &expansion) calcule la longueur d'expansion de 'lexer'.
 * 
 * @dependances: 'update_quoting_state', 'is_special_syntax_character', 'single_quote_expantion', 'basic_expantion'.
 * 
 * @pourquoi:
 *   - Calcul Exhaustif de la Longueur : Assure une estimation précise de la longueur totale 
 *     de la chaîne après expansion, essentielle pour allouer correctement la mémoire.
 *   - Gestion des Caractères Spéciaux : Traite les caractères spéciaux et les variables 
 *     d'environnement, qui peuvent modifier la longueur de la chaîne de manière significative.
 *   - Fiabilité dans les Expansions : Offre une approche robuste pour évaluer les expansions 
 *     dans divers cas, y compris ceux avec des caractères spéciaux et des guillemets, 
 *     garantissant ainsi des résultats d'expansion fiables et cohérents.
 * 
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Parcourir 'word' dans 'expnd'
 *     |
 *     v
 *   Caractère courant == '$' ?
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Traitement    Ajouter le caractère à 'exp->str'
 * du caractère  et incrémenter 'exp->len'
 * '$'
 *  |
 *  v
 * Prochain caractère spécial ou fin ?
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Ajouter '$'   Vérifier si le prochain caractère
 * à 'exp->str'   est un chiffre
 *                /         \
 *              VRAI       FAUX
 *               |           \
 *               |            \
 *               v             v
 *            Incrémenter   Vérifier si dans des guillemets simples
 *            'index'       /         \
 *                         VRAI       FAUX
 *                          |           \
 *                          |            \
 *                          v             v
 *                       Appeler       Appeler 'basic_expantion'
 *                       'single_quote_expantion'
 *                          |
 *                          v
 *                       Incrémenter 'index'
 * Fin
 */
void	sum_expansion_length(t_lexer **expnd, t_quote *state, char **nv, t_expand *exp)
{
	int	index;

	index = ZERO_INIT;
	while ((*expnd)->word[index])
	{
		update_quoting_state((*expnd)->word[index], state);
		if ((*expnd)->word[index] == '$')
		{
			if ((*expnd)->word[index + FORWARD_OFFSET] == '\0'
				|| is_special_syntax_character((*expnd)->word[index + FORWARD_OFFSET], state))
				exp->str[exp->len++] = (*expnd)->word[index];
			else if ((*expnd)->word[index + FORWARD_OFFSET] && ft_isdigit((*expnd)->word[index + FORWARD_OFFSET]))
				index++;
			else if (state->is_quote == 1 && state->sq_first == 1)
				index += single_quote_expantion(&(*expnd)->word[index], exp) IDX_ADJUST;
			else
				index += basic_expantion(&(*expnd)->word[index], exp, nv, state) IDX_ADJUST;
		}
		else
			exp->str[exp->len++] = (*expnd)->word[index];
		index++;
	}
}

/**
 * @fonction: expansion_length_for_word
 * @brief: Calcule la longueur totale après expansion pour un mot spécifique.
 * 
 * Cette fonction évalue un mot dans une structure lexer pour calculer la longueur totale
 * après expansion en tenant compte des variables d'environnement et des règles d'expansion
 * spéciales, comme le traitement des erreurs et des caractères spéciaux.
 * 
 * @pourquoi:
 *   - Précision dans le Calcul de Longueur : Cette fonction assure une évaluation précise de la longueur 
 *     nécessaire pour stocker un mot après son expansion. Cela est crucial pour éviter les dépassements 
 *     de mémoire et garantir que l'espace alloué est adéquat.
 *   - Gestion des Variables et Caractères Spéciaux : Prend en compte les variables d'environnement et 
 *     les caractères spéciaux (comme '$' et '?'), qui peuvent modifier significativement la longueur du 
 *     mot après expansion.
 *   - Flexibilité et Fidélité d'Expansion : Fournit une base fiable pour l'expansion de mots, en respectant 
 *     les règles et les cas spéciaux du shell, permettant ainsi de refléter fidèlement la manière dont 
 *     le shell interpréterait et étendrait une commande.

 * 
 * @param exp: Pointeur sur un lexer (t_lexer) à traiter.
 * @param state: État actuel des guillemets (t_quote).
 * @param env: Tableau de chaînes de caractères représentant les variables d'environnement.
 * @param expanded_length: Pointeur sur un entier pour stocker la longueur calculée.
 * 
 * @erreur: Aucune erreur spécifique n'est définie pour cette fonction.
 * 
 * @effet_de_bord: Modifie la valeur pointée par 'expanded_length'.
 * 
 * @exemple: expansion_length_for_word(&lexer, state, env, &length) calcule la longueur d'expansion pour 'lexer'.
 * 
 * @dependances: 'update_quoting_state', 'search_in_env_len', 'is_special_syntax_character'.
 * 
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Parcourir 'word' dans 'exp'
 *     |
 *     v
 *   Caractère courant == '$' ?
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Traitement    Incrémenter 'expanded_length'
 * du caractère  et 'index'
 * '$'
 *  |
 *  v
 * Prochain caractère == '?' ?
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Ajouter la   Appeler 'search_in_env_len'
 * longueur     pour chercher dans 'env'
 * d'erreur     et mettre à jour 'expanded_length'
 * à 'expanded_length'
 *  |             |
 *  |             v
 *  |          'state->found' == 0 et caractère spécial ?
 *  |         /         \
 *  |       VRAI       FAUX
 *  |        |           \
 *  |        |            \
 *  |        v             v
 *  |    Incrémenter    Continuer la boucle
 *  |    'expanded_length'   sur 'word'
 *  | 
 *  v
 * Fin de la fonction
 */
void	expansion_length_for_word(t_lexer **exp, t_quote *state, char **env, int *expanded_length)
{
	int	index;

	index = ZERO_INIT;
	while ((*exp)->word[index])
	{
		update_quoting_state((*exp)->word[index], state);
		if ((*exp)->word[index] == '$')
		{
			if ((*exp)->word[index + FORWARD_OFFSET] && (*exp)->word[index + FORWARD_OFFSET] == '?')
			{
				(*expanded_length) += ft_strlen(g_all.utils->error);
				index += SKIP_DOLLAR_QUESTION_MARK;
			}
			else
				index += search_in_env_len(&(*exp)->word[index], env, state, expanded_length);
			if (state->found == 0 && (*exp)->word[index] && is_special_syntax_character((*exp)->word[index], state))
				(*expanded_length)++;
		}
		else
		{
			(*expanded_length)++;
			index++;
		}
	}
}
