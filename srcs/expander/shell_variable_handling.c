#include "../../include/minishell.h"

bool is_next_char_valid_and_not_single_quote(const char *word, int *indx)
{
    return (word[++(*indx)] != '\0' && word[*indx] != '\'');
}


int	append_chars_expnt_until_singl_quot(char *word, t_expand *exp)
{
	int	indx;

	indx = -1;
	while (is_next_char_valid_and_not_single_quote(word, &indx))
	{
		exp->value_of_expanded_var_from_env\
		[exp->length_of_expanded_var_value++] = word[indx];
	}
	return (indx);
}

/**
 * @nom: append_chars_expnt_until_singl_quot
 * @breve_description: Effectue une expansion de chaîne de caractères en tenant
 * compte des guillemets simples.
 * 
 * @param word: char *word, pointeur vers la chaîne de caractères à traiter.
 * @param exp: t_expand *exp, pointeur vers une structure gérant l'expansion.
 * 
 * @description_detaillee:
 * La fonction 'append_chars_expnt_until_singl_quot' parcourt la chaîne 'word'
 * et ajoute chaque caractère à la chaîne de la structure 'exp', jusqu'à
 * rencontrer un guillemet simple ou la fin de la chaîne. Cette fonction est
 * utilisée pour gérer l'expansion des chaînes de caractères encadrées par des
 * guillemets simples, où l'interprétation des caractères est généralement
 * littérale.
 *   
 * Pourquoi effectuer une expansion avec des guillemets simples ?
 * - Interprétation Littérale : Dans de nombreux langages de script et shells,
 * les guillemets simples sont utilisés pour indiquer que le contenu doit être
 * traité de manière littérale, sans interprétation des caractères spéciaux 
 * ou des variables.
 * - Gestion des Chaînes de Caractères : Permet de traiter correctement les
 * chaînes encapsulées dans des guillemets simples, en conservant leur forme
 * originale.
 *
 * @valeur_de_retour: 
 * Retourne l'indice jusqu'où l'expansion a été effectuée dans la chaîne 'word'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'word' est NULL, le comportement est indéfini.
 * - Modifie 'exp' en ajoutant les caractères à 'exp->str' et en mettant à jour
 * 'exp->len'.
 *
 * @exemples_utilisation:
 * char *word = "'Ceci est un test'";
 * t_expand exp;
 * init_expand_structure(&exp);
 * int indice = append_chars_expnt_until_singl_quot(word, &exp);
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser la variable 'index' à -1
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'word'
 *     |
 *     v
 *   - Vérifier si le caractère courant 
 *               est
 * un guillemet simple ou la fin de 'word'
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Ajouter le caractère courant à 'exp->str'  - Retourner 'index'
 *   - Incrémenter 'exp->len' et 'index'
 *   |
 *   \----------------------------------/
 *   |
 *   v
 *   Fin
 */
// int	append_chars_expnt_until_singl_quot(char *word, t_expand *exp)
// {
// 	int	indx;

// 	indx = -1;
// 	while (word[++indx] && word[indx] != '\'')
// 	{
// 		exp->value_of_expanded_var_from_env[exp->length_of_expanded_var_value++] = word[indx];
// 	}
// 	return (indx);
// }

bool is_not_end_of_string_or_dollar_sign(char char_to_check)
{
    return (char_to_check != '\0' && char_to_check != '$');
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

bool	is_error_code_char_present(t_data *data, int indx)
{
	return (data->utils->g_signal_in_char_format[indx]);
}

/**
 * @nom: append_curnt_error_code_to_expansion_struc
 * @breve_description: Insère le code d'erreur courant dans la structure
 * d'expansion.
 * 
 * @param exp: t_expand *exp, pointeur vers la structure d'expansion à mettre
 * à jour.
 * 
 * @description_detaillee:
 * La fonction 'append_curnt_error_code_to_expansion_struc' ajoute le code
 * d'erreur stocké dans 'data->utils->error' à la chaîne de caractères dans la
 * structure 'exp'. Elle parcourt le code d'erreur caractère par caractère et
 * les ajoute à 'exp->str', en mettant à jour 'exp->len' pour chaque caractère
 * ajouté. Cette fonction est utilisée principalement dans le contexte de
 * l'expansion de variables dans un shell, pour gérer les erreurs d'expansion.
 *   
 * Pourquoi insérer un code d'erreur dans la chaîne d'expansion ?
 * - Gestion des Erreurs : Permet de refléter les erreurs rencontrées pendant
 * l'expansion des variables dans le résultat final, ce qui est crucial pour le
 * débogage et le traitement des erreurs dans un shell ou un script.
 * - Communication d'État : Fournit un moyen direct d'indiquer à l'utilisateur
 * ou au système qu'une erreur a eu lieu, en l'incorporant dans la chaîne de
 * sortie.
 *
 * @valeur_de_retour: 
 * Retourne un entier indiquant la position jusqu'où l'insertion a été
 * effectuée.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - La fonction modifie 'exp', en ajoutant le code d'erreur à 'exp->str' et
 * en mettant à jour 'exp->len'.
 * - Aucune vérification n'est faite pour s'assurer que 'exp->str' a
 * suffisamment de place.
 *
 * @exemples_utilisation:
 * t_expand exp;
 * init_expand_structure(&exp);
 * data->utils->error = "127";
 * int position = append_curnt_error_code_to_expansion_struc(&exp);
 *
 * @dependances: 
 * Aucune dépendance externe autre que la structure globale 'g_all'.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser la variable 'i' à -1
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'data->utils->error'
 *     |
 *     v
 *   - Vérifier si la fin du code d'erreur est atteinte
 *   /                     \
 *  NON                    OUI
 *   |                      |
 *   v                      |
 *   - Ajouter              |
 *     le caractère         |
 *     courant              |
 *     à                    v
 *     'exp->str'           - Marquer 'exp->found' comme 1 (trouvé)
 *   - Incrémenter			- Retourner la position actuelle (2)
 *     'exp->len' et 'i'              
 *   |
 *   |
 *   \----------------------------------/
 *   |
 *   v
 *   Fin
 */
int	append_curnt_error_code_to_expansion_struc(t_expand *exp, t_data *data)
{
	int	indx;

	indx = -1;
	while (is_error_code_char_present(data, ++indx))
		exp->value_of_expanded_var_from_env[\
		exp->length_of_expanded_var_value++] = \
		data->utils->g_signal_in_char_format[indx];
	exp->var_env_match_found = 1;
	return (2);
}

bool	is_matching_env_var_name(\
char *w, t_data *data, int i, int env_var_char_idx)
{
    return (w[i] && 
           data->full_env_var_copy_gamma\
		   [data->env_var_line_idx][env_var_char_idx] \
		   &&
           w[i] == \
		   data->full_env_var_copy_gamma\
		   [data->env_var_line_idx][env_var_char_idx] \
		   && w[i] != '=');
}

bool	is_word_end_or_special(const char *word, int index, t_quote *state)
{
    return (word[index] == '\0' || word[index] == '$' \
	|| is_special_syntax_character(word[index], state));
}

bool	is_env_var_name_with_equal_sign(t_data *data, int env_var_char_idx)
{
    return (data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx] \
	&& \
	data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx] == '=');
}

bool	is_env_var_value_non_empty(t_data *data, int env_var_char_idx)
{
    return (data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx++] != '\0');
}

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
		while (data->full_env_var_copy_gamma[data->env_var_line_idx]\
		[++env_var_char_idx])
			exp->value_of_expanded_var_from_env[exp->\
			length_of_expanded_var_value++] = data->full_env_var_copy_gamma\
			[data->env_var_line_idx][env_var_char_idx];
		return (i);
	}
	return (i);
}

bool	is_current_char_question_mark(const char *word, int index)
{
    return (word[index] == '?');
}

bool	is_expansion_not_found(t_expand *exp)
{
    return (exp->var_env_match_found == NOT_FOUND);
}
bool	is_valid_env_var_entry(t_data *data)
{
    return (data->full_env_var_copy_gamma[++data->env_var_line_idx] != NULL);
}

bool	is_expansion_found(t_expand *exp)
{
    return (exp->var_env_match_found == FOUND);
}

/**
 * @nom: expand_env_vars_with_question_mark_handling
 * @breve_description: Effectue une expansion basique sur une chaîne de
 * caractères.
 * 
 * @param w: char *w, pointeur vers la chaîne de caractères à étendre.
 * @param exp: t_expand *exp, pointeur vers une structure gérant l'expansion.
 * @param nv: char **nv, tableau de pointeurs vers des chaînes représentant
 * des variables.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'expand_env_vars_with_question_mark_handling' parcourt un
 * tableau de chaînes (variables) pour trouver une correspondance avec le
 * début de la chaîne 'w'. Elle utilise
 * 'find_and_expand_env_var_with_special_char' pour comparer chaque variable
 * avec 'w'. Si une correspondance est trouvée ou si un caractère spécial '?'
 * est détecté, elle effectue l'expansion nécessaire. La fonction gère
 * également les cas où aucune correspondance n'est trouvée en appelant
 * 'handle_unfound_expansion_word'.
 *   
 * Pourquoi effectuer une expansion basique ?
 * - Interprétation des Variables : Permet de remplacer les noms de variables
 * par leurs valeurs correspondantes, ce qui est essentiel pour l'exécution
 * correcte des commandes dans un shell.
 * - Gestion des Caractères Spéciaux : Traite des caractères spéciaux comme
 * '?', qui peuvent avoir une signification particulière dans certains
 * contextes, comme les opérations conditionnelles.
 *
 * @valeur_de_retour: 
 * Retourne l'indice jusqu'où l'expansion a été effectuée ou un code d'erreur
 * si aucune expansion n'est possible.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si aucune correspondance n'est trouvée, appelle
 * 'handle_unfound_expansion_word'.
 * - La fonction modifie 'exp' pour indiquer si une expansion a été trouvée.
 *
 * @exemples_utilisation:
 * char *word = "$HOME";
 * t_expand exp;
 * char *variables[] = {"HOME=/home/user", NULL};
 * t_quote state = {0, 0};
 * int indice = expand_env_vars_with_question_mark_handling(\
 * word, &exp, variables, &state);
 *
 * @dependances: 
 * - find_and_expand_env_var_with_special_char pour rechercher des
 * correspondances.
 * - handle_unfound_expansion_word pour gérer les cas sans correspondance.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser les variables 'i', 'x', et 'env_var_char_idx'
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'nv'
 *     |
 *     v
 *   - Vérifier si le caractère suivant dans 'w' est spécial ('?')
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Appeler
 * find_and_expand_env_var_with_special_char       - Comparer
 *   - Retourner le résultat                          'w[i]' avec 'nv[x][env_var_char_idx]'
 *     /       \
 *                               OUI      NON
 *                                |         |
 *                                v         v
 *   - Appeler        - Continuer avec la prochaine variable dans 'nv'
 *     find_and_expand_env_var_with_special_char
 *   - Vérifier si 'exp->found' est à 1
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         |
 *   - Retourner l'indice 'i'    |
 *   |<--------------------------|
 *   |
 *   v
 *   - Vérifier si 'exp->found' est à 0
 *   /                                  \
 *  OUI                                 NON
 *   |                                   |
 *   v                                   v
 *   - Appeler                           - Continuer le processus
 *     handle_unfound_expansion_word
 *   - Retourner le résultat
 *   |
 *   v
 *   Fin
 */
int	expand_env_vars_with_question_mark_handling(\
char *w, t_expand *exp, t_data *data, t_quote *state)
{
	int	i;
	int	env_var_char_idx;

	i = 1;
	data->env_var_line_idx = -1;
	exp->var_env_match_found = ZERO_INIT;
	while (is_valid_env_var_entry(data))
	{
		env_var_char_idx = 0;
		i = 1;
		if (is_current_char_question_mark(w, i))
			return (\
			find_and_expand_env_var_with_special_char(w, exp, data, state));
		if (is_char_matching_env_var(w, i, data->full_env_var_copy_gamma\
		[data->env_var_line_idx], env_var_char_idx))
		{
			i = find_and_expand_env_var_with_special_char(w, exp, data, state);
			if (is_expansion_found(exp))
				return (i);
		}
	}
	if (is_expansion_not_found(exp))
		return (handle_unfound_expansion_word(w, state));
	return (i);
}
