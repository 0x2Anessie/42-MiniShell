#include "../../include/minishell.h"

int	append_chars_expnt_until_singl_quot(char *word, t_expand *exp)
{
	int	indx;

	indx = -1;
	while (is_next_char_valid_and_not_single_quote(word, &indx))
	{
		exp->value_of_expanded_var_from_env[\
		exp->length_of_expanded_var_value++] = word[indx];
	}
	return (indx);
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
 *   - Retourner le résultat                        'w[i]' avec 
 *      /            \                              'nv[x][env_var_char_idx]'
 *     /              \
 *   OUI              NON
 *    |               |
 *    v               v
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
			return (find_and_expand_env_var_with_special_char(\
			w, exp, data, state));
		if (is_char_matching_env_var(\
		w, i, data->full_env_var_copy_gamma[data->env_var_line_idx], \
		env_var_char_idx))
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
