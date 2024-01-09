#include "../../include/minishell.h"

bool	is_current_char_dollar_sign(t_lexer *exp, int index)
{
    return (exp->word[index] == '$');
}


/**
 * @nom: expand_variables_and_handle_special_chars
 * @brief: Calcule la longueur totale de la chaîne après expansion.
 * 
 * @description:
 * Évalue chaque caractère d'un mot dans une structure lexer et calcule la
 * longueur totale de la chaîne après avoir pris en compte les expansions
 * nécessaires, les caractères spéciaux et les différentes règles d'expansion.
 * 
 * @param expnd: Pointeur sur un lexer (t_lexer) à traiter pour l'expansion.
 * @param state: État actuel des guillemets (t_quote).
 * @param nv: Tableau de chaînes de caractères représentant les variables
 * d'environnement.
 * @param exp: Structure représentant l'état de l'expansion (t_expand).
 * 
 * @erreur: Aucune erreur spécifique n'est définie pour cette fonction.
 * 
 * @effet_de_bord: Modifie la longueur totale dans 'exp->len' et le contenu
 * de 'exp->str'.
 * 
 * @exemple: expand_variables_and_handle_special_chars(&lexer, state, env,
 * &expansion) calcule la longueur d'expansion de 'lexer'.
 * 
 * @dependances: 'update_quoting_state', 'is_special_syntax_character',
 * 'append_chars_expnt_until_singl_quot',
 * 'expand_env_vars_with_question_mark_handling'.
 * 
 * @pourquoi:
 * - Calcul Exhaustif de la Longueur : Assure une estimation précise de la
 * longueur totale de la chaîne après expansion, essentielle pour allouer
 * correctement la mémoire.
 * - Gestion des Caractères Spéciaux : Traite les caractères spéciaux et les
 * variables d'environnement, qui peuvent modifier la longueur de la chaîne de
 * manière significative.
 * - Fiabilité dans les Expansions : Offre une approche robuste pour évaluer
 * les expansions dans divers cas, y compris ceux avec des caractères spéciaux
 * et des guillemets, garantissant ainsi des résultats d'expansion fiables et
 * cohérents.
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
 *           Incrémenter    Vérifier si dans des guillemets simples
 *            'index'       /         \
 *                        VRAI        FAUX
 *                        /             \
 *                       /               \
 *                      /                 \
 *                     v                   v
 *                  Appeler             Appeler 
 *              'append_chars_expnt_until_singl_quot'
 *                               |
 *                               v
 *                      Incrémenter 'index'
 * Fin
 */
void	expand_variables_and_handle_special_chars(\
t_lexer **expnd, t_quote *state, t_expand *exp, t_data *data)
{
	int	index;

	index = ZERO_INIT;
	while ((*expnd)->word[index])
	{
		update_quoting_state((*expnd)->word[index], state);
		if (is_current_char_dollar_sign(*expnd, index))
		{
			if (is_next_char_end_or_special(&(*expnd)->word[index], state))
				exp->value_of_expanded_var_from_env[exp->length_of_expanded_var_value++] = (*expnd)->word[index];
			else if (is_next_char_decimal_digit(&(*expnd)->word[index]))
				index++;
			else if (is_single_quote_open_and_started(state))
				index += append_chars_expnt_until_singl_quot(\
				&(*expnd)->word[index], exp) IDX_ADJUST;
			else
				index += expand_env_vars_with_question_mark_handling(\
				&(*expnd)->word[index], exp, data, state) IDX_ADJUST;
		}
		else
			exp->value_of_expanded_var_from_env[exp->length_of_expanded_var_value++] = (*expnd)->word[index];
		index++;
	}
}

/**
 * @fonction: length_of_env_variable_value
 * @brief: Calcule la longueur totale après expansion pour un mot spécifique.
 * 
 * Cette fonction évalue un mot dans une structure lexer pour calculer la
 * longueur totale après expansion en tenant compte des variables
 * d'environnement et des règles d'expansion spéciales, comme le traitement des
 * erreurs et des caractères spéciaux.
 * 
 * @pourquoi:
 * - Précision dans le Calcul de Longueur : Cette fonction assure une
 * évaluation précise de la longueur nécessaire pour stocker un mot après son
 * expansion. Cela est crucial pour éviter les dépassements de mémoire et
 * garantir que l'espace alloué est adéquat.
 * - Gestion des Variables et Caractères Spéciaux : Prend en compte les
 * variables d'environnement et les caractères spéciaux (comme '$' et '?'),
 * qui peuvent modifier significativement la longueur du mot après expansion.
 * - Flexibilité et Fidélité d'Expansion : Fournit une base fiable pour
 * l'expansion de mots, en respectant les règles et les cas spéciaux du shell,
 * permettant ainsi de refléter fidèlement la manière dont le shell
 * interpréterait et étendrait une commande.

 * 
 * @param exp: Pointeur sur un lexer (t_lexer) à traiter.
 * @param state: État actuel des guillemets (t_quote).
 * @param env: Tableau de chaînes de caractères représentant les variables
 * d'environnement.
 * @param expanded_length: Pointeur sur un entier pour stocker la longueur
 * calculée.
 * 
 * @erreur: Aucune erreur spécifique n'est définie pour cette fonction.
 * 
 * @effet_de_bord: Modifie la valeur pointée par 'expanded_length'.
 * 
 * @exemple: length_of_env_variable_value(&lexer, state, env, &length)
 * calcule la longueur d'expansion pour 'lexer'.
 * 
 * @dependances: 'update_quoting_state', 'calculate_expanded_env_var_length',
 * 'is_special_syntax_character'.
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
 * Ajouter la   Appeler 'calculate_expanded_env_var_length'
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
void	length_of_env_variable_value(\
t_lexer **exp, t_quote *state, t_data *data, int *expanded_length)
{
	int	index;

	index = ZERO_INIT;
	while ((*exp)->word[index])
	{
		update_quoting_state((*exp)->word[index], state);
		if (is_current_char_dollar_sign(*exp, index)) 
		{
			if (is_next_char_question_mark(*exp, index))
			{
				(*expanded_length) += ft_strlen(data->utils->g_signal_in_char_format);
				index += SKIP_DOLLAR_QUESTION_MARK;
			}
			else
				index += calculate_expanded_env_var_length(\
				&(*exp)->word[index], data->full_env_var_copy_alpha, state, expanded_length);
			if (is_special_char_found_with_state_not_found(*exp, state, index))
				(*expanded_length)++;
		}
		else
		{
			(*expanded_length)++;
			index++;
		}
	}
}
