#include "../../include/minishell.h"

/**
 * @nom: determine_expansion_or_quote_removal
 * @brief: Détermine si un mot doit être étendu ou si les guillemets doivent
 * être retirés.
 *
 * @description:
 * Analyse le mot contenu dans une structure lexer pour décider si une
 * expansion est nécessaire ou si les guillemets doivent être supprimés.
 * Cette fonction est cruciale dans le processus d'interprétation des commandes
 * d'un shell, où la présence de symboles spéciaux comme '$' ou de guillemets
 * influe sur le traitement du mot.
 *
 * @param to_check: t_lexer **to_check, double pointeur vers une structure
 * lexer contenant le mot à analyser.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * @param exp: t_expand *exp, pointeur vers une structure représentant l'état
 * de l'expansion.
 *
 * @pourquoi:
 * - Gestion des Symboles et Guillemets : Permet d'identifier et de traiter
 * correctement les symboles spéciaux et les guillemets dans les mots, 
 * affectant leur expansion.
 * - Préparation à l'Expansion : Détermine si les conditions sont remplies
 * pour une expansion des variables ou si des modifications syntaxiques
 * (comme la suppression des guillemets) sont nécessaires.
 *
 * @valeur_de_retour:
 * Retourne EXPANSION_REQUIRED si une expansion est nécessaire,
 * NO_ACTION_REQUIRED si aucune action n'est requise, ou NO_EXPAND si le mot
 * est un délimiteur qui ne doit pas être étendu.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Modifie la structure lexer pointée par 'to_check' si les guillemets sont
 * retirés.
 * - Met à jour les structures 'state' et 'exp' selon l'analyse du mot.
 *
 * @exemples_utilisation:
 * t_lexer *lexer = create_lexer("$VAR 'texte'");
 * t_quote state;
 * t_expand exp;
 * int action = determine_expansion_or_quote_removal(&lexer, &state, &exp);
 *
 * @dependances:
 * Utilise 'create_cleaned_str_excluding_inactive_quots' pour retirer les
 * guillemets si nécessaire.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
  *   - Initialiser 'exp->need_expand' et 'exp->quote'
 *     |
 *     v
 *   Parcourir 'word'
 *     |
 *     v
 *   Token == DELIMITER ?
 *  /         \
 * VRAI       FAUX
 *  |           \
 *  |            \
 *  v             v
 * Retourne    Caractère '$' trouvé ?
 * NO_EXPAND   /         \
 *            VRAI       FAUX
 *             |           \
 *             |            \
 *             v             v
 *          Marquer       Caractère '"' ou ''' trouvé ?
 *          NEED_EXPAND   /         \
 *                       VRAI       FAUX
 *                        |           \
 *                        |            \
 *                        v             v
 *                     Marquer        Retourne
 *                     QUOTED         EXPANSION_REQUIRED
 *                        |
 *                        v
 *                    Besoin d'expansion et guillemets trouvés ?
 *                   /         \
 *                 VRAI       FAUX
 *                  |           \
 *                  |            \
 *                  v             v
 *               Retirer        Retourne
 *               guillemets     NO_ACTION_REQUIRED
 *                  |
 *                  v
 *               Retourne
 *               NO_ACTION_REQUIRED
 */
// int	determine_expansion_or_quote_removal(\
// t_lexer **to_check, t_quote *state, t_expand *exp, t_data *data)
// {
// 	int		index;

// 	exp->need_expand = ZERO_INIT;
// 	exp->quote = ZERO_INIT;
// 	index = ZERO_INIT;
// 	while ((*to_check)->word[index])/*         ---> condition non intelligible --> fonction         */
// 	{
// 		if ((*to_check)->token == DELIMITER)/*         ---> condition non intelligible --> fonction         */
// 			return (NO_EXPAND);
// 		if ((*to_check)->word[index] == '$')/*         ---> condition non intelligible --> fonction         */
// 			exp->need_expand = NEED_EXPAND;
// 		if ((*to_check)->word[index] == '"' || (*to_check)->word[index] == '\'')/*         ---> condition non intelligible --> fonction         */
// 			exp->quote = QUOTED;
// 		index++;
// 	}
// 	if (exp->need_expand == NO_EXPAND && exp->quote == QUOTED)/*         ---> condition non intelligible --> fonction         */
// 		(*to_check)->word = \
// 		create_cleaned_str_excluding_inactive_quots(\
// 		(*to_check)->word, state, data);
// 	else
// 		return (EXPANSION_REQUIRED);
// 	return (NO_ACTION_REQUIRED);
// }

// bool	should_expand(t_lexer *tmp, t_quote *state, t_expand *exp, t_data *data)
// {
// 	t_lexer	*save;

// 	save = tmp;
// 	if (is_dollar_or_doubl_or_singl_quote(tmp->word))
// 	{
// 		reset_quoting_state(state);
// 		if (!is_dollar_at_end(save->word))
// 			return (false);
// 		return (determine_expansion_or_quote_removal(&save, state, exp, data));
// 	}
// 	return (false);
// }

// /**
//  * @nom: expand
//  * @brief: Effectue l'expansion de mots dans une structure lexer.
//  *
//  * Cette fonction parcourt les éléments d'une structure lexer (t_lexer) et,
//  * en fonction de la présence de caractères spéciaux et d'autres critères,
//  * effectue l'expansion des mots. Elle gère l'allocation et l'initialisation
//  * des structures d'état des guillemets (t_quote) et d'expansion (t_expand), et
//  * appelle différentes fonctions auxiliaires pour l'expansion des mots et la
//  * gestion des guillemets.
//  *
//  * @pourquoi: 
//  * - L'expansion de mots est un élément clé dans l'interprétation des commandes
//  * dans un shell, permettant de traiter correctement les variables, les
//  * guillemets et d'autres constructions syntaxiques.
//  * - Fournit une flexibilité et une puissance accrues dans le traitement des
//  * commandes, en permettant des substitutions et des manipulations dynamiques
//  * de chaînes.
//  *
//  * @param state: t_quote *state, pointeur vers l'état des guillemets.
//  * @param env: char **env, tableau de chaînes de caractères représentant
//  * l'environnement.
//  * @param tmp: t_lexer *tmp, pointeur vers le lexer à traiter.
//  *
//  * @gestion_des_erreurs: Si l'allocation pour 'state' ou 'exp' échoue, la
//  * fonction se termine prématurément.
//  *
//  * @effet_de_bord: Modifie la structure du lexer et l'état des guillemets.
//  *
//  * @exemple: 
//  *   t_quote *state = NULL;
//  *   char *env[] = {"VAR=value", NULL};
//  *   t_lexer *lexer = create_lexer("echo $VAR");
//  *   expand(state, env, lexer);
//  *   // Traite chaque mot dans 'lexer' pour l'expansion.
//  *
//  * @dependances: Utilise 'is_dollar_or_doubl_or_singl_quote',
//  * 'is_dollar_at_end', 'determine_expansion_or_quote_removal', et
//  * 'expand_and_insert_in_lexeme_linked_list' pour diverses opérations
//  * d'expansion.
//  *
//  * @graphe_de_flux:
//  *   Début
//  *     |
//  *     v
//  *   - Allocation de 'state' et 'exp'
//  *     |
//  *     v
//  *   - Parcourir 'tmp'
//  *     |
//  *     v
//  *   - 'tmp' est-il non nul ?
//  *   /       \
//  * VRAI     FAUX
//  *   |         |
//  *   v         v
//  *   - Sauvegarde 'tmp'   - Fin de la fonction
//  *     dans 'save'
//  *     |
//  *     v
//  *   - Caractère spécial dans 'save->word' ?
//  *   /       \
//  * VRAI     FAUX
//  *   |         |
//  *   v         v
//  *   - Gestion de   - Passage au mot suivant
//  *     l'expansion
//  *     |
//  *     v
//  *   - Passage au mot suivant
//  *     |
//  *     v
//  *   Fin
//  */
// void	expand(t_quote *state, char **env, t_lexer *tmp, t_data *data)
// {
// 	t_lexer		*save;
// 	t_expand	*exp;

// 	data->nv = env;
// 	state = ft_malloc_with_tracking(data, sizeof(t_quote));
// 	if (!state)
// 		return ;
// 	reset_quoting_state(state);
// 	exp = ft_malloc_with_tracking(data, sizeof(t_expand));
// 	if (!exp)
// 		return ;
// 	while (tmp)
// 	{
// 		save = tmp;
// 		if (should_expand(tmp, state, exp, data))
// 			expand_and_insert_in_lexeme_linked_list(&tmp, state, data, exp);
// 		else
// 			tmp = tmp->next;
// 	}
// }

int	determine_expansion_or_quote_removal(t_lexer **to_check, t_quote *state, t_expand *exp, t_data *data)
{
	int		index;

	exp->need_expand = ZERO_INIT;
	exp->quote = ZERO_INIT;
	index = ZERO_INIT;
	while ((*to_check)->word[index])
	{
		if ((*to_check)->token == DELIMITER)
			return (NO_EXPAND);
		if ((*to_check)->word[index] == '$')
			exp->need_expand = NEED_EXPAND;
		if ((*to_check)->word[index] == '"' || (*to_check)->word[index] == '\'')
			exp->quote = QUOTED;
		index++;
	}
	if (exp->need_expand == NO_EXPAND && exp->quote == QUOTED)
		(*to_check)->word = create_cleaned_str_excluding_inactive_quots((*to_check)->word, state, data);
	else
		return (EXPANSION_REQUIRED);
	return (NO_ACTION_REQUIRED);
}

/**
 * @nom: expand
 * @brief: Effectue l'expansion de mots dans une structure lexer.
 *
 * Cette fonction parcourt les éléments d'une structure lexer (t_lexer) et, en fonction 
 * de la présence de caractères spéciaux et d'autres critères, effectue l'expansion des mots. 
 * Elle gère l'allocation et l'initialisation des structures d'état des guillemets (t_quote) 
 * et d'expansion (t_expand), et appelle différentes fonctions auxiliaires pour l'expansion 
 * des mots et la gestion des guillemets.
 *
 * @pourquoi: 
 *   - L'expansion de mots est un élément clé dans l'interprétation des commandes dans un shell, 
 *     permettant de traiter correctement les variables, les guillemets et d'autres constructions syntaxiques.
 *   - Fournit une flexibilité et une puissance accrues dans le traitement des commandes, 
 *     en permettant des substitutions et des manipulations dynamiques de chaînes.
 *
 * @param state: t_quote *state, pointeur vers l'état des guillemets.
 * @param env: char **env, tableau de chaînes de caractères représentant l'environnement.
 * @param tmp: t_lexer *tmp, pointeur vers le lexer à traiter.
 *
 * @gestion_des_erreurs: Si l'allocation pour 'state' ou 'exp' échoue, la fonction se termine prématurément.
 *
 * @effet_de_bord: Modifie la structure du lexer et l'état des guillemets.
 *
 * @exemple: 
 *   t_quote *state = NULL;
 *   char *env[] = {"VAR=value", NULL};
 *   t_lexer *lexer = create_lexer("echo $VAR");
 *   expand(state, env, lexer);
 *   // Traite chaque mot dans 'lexer' pour l'expansion.
 *
 * @dependances: Utilise 'is_dollr_quot_apstrph', 'dollar_at_end', 'determine_expansion_or_quote_removal', 
 *               et 'expand_and_insert_in_lexeme_linked_list' pour diverses opérations d'expansion.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Allocation de 'state' et 'exp'
 *     |
 *     v
 *   - Parcourir 'tmp'
 *     |
 *     v
 *   - 'tmp' est-il non nul ?
 *   /       \
 * VRAI     FAUX
 *   |         |
 *   v         v
 *   - Sauvegarde 'tmp'   - Fin de la fonction
 *     dans 'save'
 *     |
 *     v
 *   - Caractère spécial dans 'save->word' ?
 *   /       \
 * VRAI     FAUX
 *   |         |
 *   v         v
 *   - Gestion de   - Passage au mot suivant
 *     l'expansion
 *     |
 *     v
 *   - Passage au mot suivant
 *     |
 *     v
 *   Fin
 */
void	expand(t_quote *state, char **env, t_lexer *tmp, t_data *data)
{
	t_lexer		*save;
	t_expand	*exp;

	data->nv = env;
	state = ft_malloc_with_tracking(data, sizeof(t_quote));
	if (!state)
		return ;
	reset_quoting_state(state);
	exp = ft_malloc_with_tracking(data, sizeof(t_expand));
	if (!exp)
		return ;
	while (tmp)
	{
		save = tmp;
		if (is_dollar_or_doubl_or_singl_quote(tmp->word))
		{
			tmp = tmp->next;
			reset_quoting_state(state);
			if (!is_dollar_at_end(save->word))
				continue ;
			if (determine_expansion_or_quote_removal(&save, state, exp, data))
				expand_and_insert_in_lexeme_linked_list(&save, state, data, exp);
		}
		else
			tmp = tmp->next;
	}
}