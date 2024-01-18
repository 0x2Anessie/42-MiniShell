#include "../../include/minishell.h"

int	find_and_expand_env_var_with_special_char(\
char *w, t_expand *exp, t_data *data, t_quote *state)
{
	int	i;
	int	env_var_char_idx;

	i = -1;
	env_var_char_idx = ZERO_INIT;
	if (w[1] == '?')
		return (append_curnt_error_code_to_expansion_struc(exp, data));
	i = 1;
	while (is_matching_env_var_name(w, data, i, env_var_char_idx))
	{
		env_var_char_idx++;
		i++;
	}
	if (is_word_end_or_special(w, i, state) \
	&& is_env_var_name_with_equal_sign(data, env_var_char_idx))
	{
		exp->var_env_match_found = 1;
		while (data->full_env_var_copy_gamma[data->\
		env_var_line_idx][++env_var_char_idx])
			exp->value_of_expanded_var_from_env[exp->\
			length_of_expanded_var_value++] = data->\
			full_env_var_copy_gamma[data->env_var_line_idx][env_var_char_idx];
		return (i);
	}
	return (i);
}

/**
 * @nom: handle_unfound_expansion_word
 * @breve_description: Traite le cas où un mot d'expansion n'est pas trouvé.
 * 
 * @param w: char *w, pointeur vers la chaîne de caractères contenant le mot à
 * traiter.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'handle_unfound_expansion_word' est appelée lorsqu'un mot
 * d'expansion n'est pas trouvé dans une chaîne. Elle parcourt la chaîne 'w'
 * à partir de l'indice 1, en recherchant soit la fin de la chaîne, soit le
 * caractère '$', soit un caractère spécial traité par
 * 'is_special_syntax_character'. La fonction sert à déterminer jusqu'où aller
 * dans la chaîne en cas d'échec d'expansion.
 *   
 * Pourquoi gérer les cas de mots non trouvés ?
 * - Éviter des Traitements Incorrects : Empêche l'expansion de parties de la
 * chaîne qui ne correspondent pas à des variables d'environnement valides,
 * assurant une interprétation correcte.
 * - Gestion des Caractères Spéciaux : Permet de traiter correctement les
 * caractères ayant une signification syntaxique spéciale, évitant ainsi des
 * erreurs de traitement.
 *
 * @valeur_de_retour: 
 * Retourne l'indice jusqu'où la recherche a été effectuée dans la chaîne 'w'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'w' est NULL, le comportement est indéfini.
 * - La fonction ne modifie ni 'w' ni 'state'.
 *
 * @exemples_utilisation:
 * char *word = "$varNonExistante";
 * t_quote state = {0, 0};
 * int indice = handle_unfound_expansion_word(word, &state);
 *
 * @dependances: 
 * - is_special_syntax_character pour vérifier les caractères spéciaux.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser la variable 'i' à 1
 *   |
 *   v
 *   - Vérifier si 'w[i]' n'est pas nul
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Entrer dans une boucle    - Retourner 'i'
 *     pour parcourir 'w'
 *     |
 *     v
 *   - Vérifier si 'w[i]' est '$' ou la fin de la chaîne
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Vérifier si 'w[i]' est un  - Retourner 'i'
 *     caractère spécial
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Incrémenter 'i'            - Retourner 'i'
 *   |
 *   \----------------------------/
 *   |
 *   v
 *   Fin
 */
int	handle_unfound_expansion_word(char *w, t_quote *state)
{
	int	indx;

	indx = 1;
	if (w[indx])
	{
		while ((is_not_end_of_string_or_dollar_sign(w[indx])))
		{
			if (is_special_syntax_character(w[indx], state))
				return (indx);
			indx++;
		}
	}
	return (indx);
}
