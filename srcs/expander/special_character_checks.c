#include "../../include/minishell.h"

int is_special_syntax_character(char c, t_quote *st) {
    char special_chars[] = ":=%^+~/'\"@]";
    int index;

    index = ZERO_INIT;

    // Si on est à l'intérieur de guillemets, certains caractères ne sont pas considérés comme spéciaux
    if (st->singl_quot_status == SINGLE_QUOTE_STARTED || st->doubl_quot_status == DOUBL_QUOT_OPEN) {
        char reduced_special_chars[] = "%^~@]";
        for (index = 0; reduced_special_chars[index]; index++) {
            if (c == reduced_special_chars[index])
                return CHAR_IS_SPECIAL;
        }
        return CHAR_IS_NOT_SPECIAL;
    }

    // Comportement par défaut hors des guillemets
    for (index = 0; special_chars[index]; index++) {
        if (c == special_chars[index])
            return CHAR_IS_SPECIAL;
    }
    return CHAR_IS_NOT_SPECIAL;
}


/**
 * @nom: is_special_syntax_character
 * @brief: Vérifie si un caractère est un caractère spécial de syntaxe.
 *
 * @description:
 * Examine si le caractère donné 'c' fait partie d'un ensemble de caractères
 * spéciaux définis, qui sont significatifs dans le contexte d'analyse
 * syntaxique d'un shell ou d'un interpréteur de commandes. Cette fonction est
 * utile pour identifier les caractères qui peuvent changer la signification
 * d'une chaîne ou nécessiter un traitement spécial.
 *
 * @param c:
 * char c, le caractère à vérifier.
 * 
 * @param st: 
 * t_quote *st, pointeur vers une structure représentant l'état des guillemets 
 * (non utilisé dans cette fonction, inclus pour la compatibilité d'interface).
 *
 * @pourquoi:
 * - Distinction de Syntaxe Spéciale : Identifier les caractères spéciaux
 * permet de gérer correctement les constructions syntaxiques complexes dans
 * l'interprétation des commandes.
 * - Précision dans l'Analyse : Assure que les caractères ayant une
 * signification particulière sont traités de manière appropriée, évitant des
 * erreurs d'interprétation.
 *
 * @valeur_de_retour:
 * Retourne CHAR_IS_SPECIAL si 'c' est un caractère spécial, sinon
 * CHAR_IS_NOT_SPECIAL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - La fonction ne modifie aucun paramètre et n'a pas d'effet de bord.
 *
 * @exemples_utilisation:
 * char c1 = '%';
 * int result1 = is_special_syntax_character(c1, NULL); 
 *       |
 *        \
 *         ---- > Retourne CHAR_IS_SPECIAL
 *   
 *   char c2 = 'a';
 *   int result2 = is_special_syntax_character(c2, NULL);
 *       |
 *        \
 *         ---- >  Retourne CHAR_IS_NOT_SPECIAL
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Initialiser 'index' à ZERO_INIT
 *     |
 *     v
 *   - Parcourir le tableau de caractères spéciaux 'special_chars'
 *     |
 *     v
 *   - Vérifier si 'c' est égal au caractère spécial courant
 *   /                            \
 *  OUI                           NON
 *   |                             |
 *   v                             v
 *   - Retourner CHAR_IS_SPECIAL   - Continuer la boucle
 *   |
 *   v
 *   Fin (Retourner CHAR_IS_NOT_SPECIAL)
 */
// int	is_special_syntax_character(char c, t_quote *st)
// {
// 	char	special_chars[] = ":=%^+~/'\"@]";
// 	int					index;

// 	index = ZERO_INIT;
// 	(void)st;
// 	while (special_chars[index])
// 	{
// 		if (c == special_chars[index])
// 			return (CHAR_IS_SPECIAL);
// 		index++;
// 	}
// 	return (CHAR_IS_NOT_SPECIAL);
// }

/**
 * @nom: is_dollar_or_doubl_or_singl_quote
 * @brief: Vérifie la présence de caractères spéciaux dans une chaîne.
 *
 * @description:
 * Parcourt une chaîne de caractères pour vérifier la présence de
 * caractères spéciaux 
 * tels que le dollar ('$'), les guillemets doubles ('"') ou les guillemets
 * simples ('\''). Cette fonction est utilisée pour identifier les chaînes
 * nécessitant une interprétation ou un traitement spéciaux, comme dans le
 * processus d'expansion de variables dans un shell.
 *
 * @param word: char *word, pointeur vers la chaîne à analyser.
 *
 * @pourquoi:
 * - Identification de Caractères Significatifs : Détecte les caractères qui
 * changent la signification d'une chaîne dans un contexte de shell,
 * influençant la manière dont elle doit être traitée.
 * - Préparation pour l'Expansion : Aide à déterminer si une chaîne contient
 * des éléments pouvant nécessiter une expansion ou une interprétation
 * spéciale.
 *
 * @valeur_de_retour:
 * Retourne CHAR_IS_SPECIAL si un caractère spécial est trouvé, sinon
 * CHAR_IS_NOT_SPECIAL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'word' est NULL, le comportement est indéfini.
 * - La fonction ne modifie pas 'word'.
 *
 * @exemples_utilisation:
 * char *example1 = "echo $VAR";
 * int result1 = is_dollar_or_doubl_or_singl_quote(example1);
 * ----------> Retourne CHAR_IS_SPECIAL
 *   
 * char *example2 = "echo text";
 * int result2 = is_dollar_or_doubl_or_singl_quote(example2);
 * ----------> Retourne CHAR_IS_NOT_SPECIAL
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Initialiser 'index' à ZERO_INIT
 *     |
 *     v
 *   - Parcourir 'word'
 *     |
 *     v
 *   - Vérifier si le caractère courant est spécial ('$' ou '"' ou '\'')
 *   /                           \
 *  OUI                          NON
 *   |                             |
 *   v                             v
 *   - Retourner CHAR_IS_SPECIAL   - Continuer la boucle
 *   |
 *   v
 *   Fin (Retourner CHAR_IS_NOT_SPECIAL)
 */
int	is_dollar_or_doubl_or_singl_quote(char *word)
{
	int	index;

	index = ZERO_INIT;
	while (word[index])
	{
		if (word[index] == '$' || word[index] == '"' || word [index] == '\'')/*         ---> condition non intelligible --> fonction         */
			return (CHAR_IS_SPECIAL);
		index++;
	}
	return (CHAR_IS_NOT_SPECIAL);
}

/**
 * @nom: is_dollar_at_end
 * @brief: Vérifie si un symbole dollar se trouve à la fin d'une chaîne de
 * caractères.
 *
 * @description:
 * Parcourt une chaîne de caractères 'str' pour déterminer si le dernier
 * caractère est un symbole dollar ('$'). Cette vérification est utile dans le
 * contexte de l'interprétation des chaînes de caractères dans un shell, où
 * le symbole dollar peut avoir une signification spéciale pour l'expansion de
 * variables.
 *
 * @param str: char *str, pointeur vers la chaîne de caractères à vérifier.
 *
 * @pourquoi:
 * - Identification de Variables : Dans un shell, un dollar à la fin d'une
 * chaîne peut indiquer une variable non terminée ou une erreur de syntaxe.
 * - Précision de l'Analyse : Permet de distinguer les cas où le symbole
 * dollar est utilisé littéralement ou comme partie d'une variable,
 * influençant le traitement de la chaîne.
 *
 * @valeur_de_retour:
 * Retourne CHAR_IS_DOLLAR si un dollar est trouvé à la fin, sinon
 * CHAR_IS_NOT_DOLLAR.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'str' est NULL ou vide, la fonction retournera CHAR_IS_NOT_DOLLAR.
 * - La fonction ne modifie pas la chaîne d'entrée.
 *
 * @exemples_utilisation:
 * char *example1 = "text$";
 * int result1 = is_dollar_at_end(example1); // Retourne CHAR_IS_DOLLAR
 *   
 * char *example2 = "text";
 * int result2 = is_dollar_at_end(example2); // Retourne CHAR_IS_NOT_DOLLAR
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   - Initialiser 'index' à ZERO_INIT
 *     |
 *     v
 *   - Parcourir 'str' jusqu'à la fin
 *     |
 *     v
 *   - Vérifier si le dernier caractère est '$'
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Retourner CHAR_IS_DOLLAR   - Retourner CHAR_IS_NOT_DOLLAR
 *   |
 *   v
 *   Fin
 */
int	is_dollar_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '$')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
}
