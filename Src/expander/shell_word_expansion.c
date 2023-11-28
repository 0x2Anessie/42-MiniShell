/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_word_expansion.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 12:57:42 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/26 22:40:10 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @nom: is_special_syntax_character
 * @brief: Vérifie si un caractère est un caractère spécial de syntaxe.
 *
 * @description:
 *   Examine si le caractère donné 'c' fait partie d'un ensemble de caractères spéciaux 
 *   définis, qui sont significatifs dans le contexte d'analyse syntaxique d'un shell ou 
 *   d'un interpréteur de commandes. Cette fonction est utile pour identifier les caractères 
 *   qui peuvent changer la signification d'une chaîne ou nécessiter un traitement spécial.
 *
 * @param c: char c, le caractère à vérifier.
 * @param st: t_quote *st, pointeur vers une structure représentant l'état des guillemets 
 *            (non utilisé dans cette fonction, inclus pour la compatibilité d'interface).
 *
 * @pourquoi:
 *   - Distinction de Syntaxe Spéciale : Identifier les caractères spéciaux permet de 
 *     gérer correctement les constructions syntaxiques complexes dans l'interprétation 
 *     des commandes.
 *   - Précision dans l'Analyse : Assure que les caractères ayant une signification particulière 
 *     sont traités de manière appropriée, évitant des erreurs d'interprétation.
 *
 * @valeur_de_retour:
 *   Retourne CHAR_IS_SPECIAL si 'c' est un caractère spécial, sinon CHAR_IS_NOT_SPECIAL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction ne modifie aucun paramètre et n'a pas d'effet de bord.
 *
 * @exemples_d'utilisation:
 *   char c1 = '%';
 *   int result1 = is_special_syntax_character(c1, NULL); // Retourne CHAR_IS_SPECIAL
 *   
 *   char c2 = 'a';
 *   int result2 = is_special_syntax_character(c2, NULL); // Retourne CHAR_IS_NOT_SPECIAL
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
int is_special_syntax_character(char c, t_quote *st)
{
    static const char special_chars[] = ":=%^+~/'\"@]";
    int index;

	index = ZERO_INIT;
    (void)st;
    while (special_chars[index])
	{
        if (c == special_chars[index])
            return (CHAR_IS_SPECIAL);
        index++;
    }
    return (CHAR_IS_NOT_SPECIAL);
}

/**
 * @nom: is_dollr_quot_apstrph
 * @brief: Vérifie la présence de caractères spéciaux dans une chaîne.
 *
 * @description:
 *   Parcourt une chaîne de caractères pour vérifier la présence de caractères spéciaux 
 *   tels que le dollar ('$'), les guillemets doubles ('"') ou les guillemets simples ('\''). 
 *   Cette fonction est utilisée pour identifier les chaînes nécessitant une interprétation 
 *   ou un traitement spéciaux, comme dans le processus d'expansion de variables dans un shell.
 *
 * @param word: char *word, pointeur vers la chaîne à analyser.
 *
 * @pourquoi:
 *   - Identification de Caractères Significatifs : Détecte les caractères qui changent la 
 *     signification d'une chaîne dans un contexte de shell, influençant la manière dont 
 *     elle doit être traitée.
 *   - Préparation pour l'Expansion : Aide à déterminer si une chaîne contient des éléments 
 *     pouvant nécessiter une expansion ou une interprétation spéciale.
 *
 * @valeur_de_retour:
 *   Retourne CHAR_IS_SPECIAL si un caractère spécial est trouvé, sinon CHAR_IS_NOT_SPECIAL.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'word' est NULL, le comportement est indéfini.
 *   - La fonction ne modifie pas 'word'.
 *
 * @exemples_d'utilisation:
 *   char *example1 = "echo $VAR";
 *   int result1 = is_dollr_quot_apstrph(example1); // Retourne CHAR_IS_SPECIAL
 *   
 *   char *example2 = "echo text";
 *   int result2 = is_dollr_quot_apstrph(example2); // Retourne CHAR_IS_NOT_SPECIAL
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
static int	is_dollr_quot_apstrph(char *word)
{
	int	index;

	index = ZERO_INIT;
	while (word[index])
	{
		if (word[index] == '$' || word[index] == '"' || word [index] == '\'')
			return (CHAR_IS_SPECIAL);
		index++;
	}
	return (CHAR_IS_NOT_SPECIAL);
}

/**
 * @nom: need_expand_or_rm_quote
 * @brief: Détermine si un mot doit être étendu ou si les guillemets doivent être retirés.
 *
 * @description:
 *   Analyse le mot contenu dans une structure lexer pour décider si une expansion est nécessaire 
 *   ou si les guillemets doivent être supprimés. Cette fonction est cruciale dans le processus 
 *   d'interprétation des commandes d'un shell, où la présence de symboles spéciaux comme '$' 
 *   ou de guillemets influe sur le traitement du mot.
 *
 * @param to_check: t_lexer **to_check, double pointeur vers une structure lexer contenant le mot à analyser.
 * @param state: t_quote *state, pointeur vers une structure représentant l'état des guillemets.
 * @param exp: t_expand *exp, pointeur vers une structure représentant l'état de l'expansion.
 *
 * @pourquoi:
 *   - Gestion des Symboles et Guillemets : Permet d'identifier et de traiter correctement 
 *     les symboles spéciaux et les guillemets dans les mots, affectant leur expansion.
 *   - Préparation à l'Expansion : Détermine si les conditions sont remplies pour une expansion 
 *     des variables ou si des modifications syntaxiques (comme la suppression des guillemets) 
 *     sont nécessaires.
 *
 * @valeur_de_retour:
 *   Retourne EXPANSION_REQUIRED si une expansion est nécessaire, NO_ACTION_REQUIRED si aucune action n'est requise, 
 *   ou NO_EXPAND si le mot est un délimiteur qui ne doit pas être étendu.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Modifie la structure lexer pointée par 'to_check' si les guillemets sont retirés.
 *   - Met à jour les structures 'state' et 'exp' selon l'analyse du mot.
 *
 * @exemples_d'utilisation:
 *   t_lexer *lexer = create_lexer("$VAR 'texte'");
 *   t_quote state;
 *   t_expand exp;
 *   int action = need_expand_or_rm_quote(&lexer, &state, &exp);
 *
 * @dependances: Utilise 'remove_quote' pour retirer les guillemets si nécessaire.
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
 *   Token == LIMITOR ?
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
int	need_expand_or_rm_quote(t_lexer **to_check, t_quote *state, t_expand *exp)
{
	int		index;

	exp->need_expand = ZERO_INIT;
	exp->quote = ZERO_INIT;
	index = ZERO_INIT;
	while ((*to_check)->word[index])
	{
		if ((*to_check)->token == LIMITOR)
			return (NO_EXPAND);
		if ((*to_check)->word[index] == '$')
			exp->need_expand = NEED_EXPAND;
		if ((*to_check)->word[index] == '"' || (*to_check)->word[index] == '\'')
			exp->quote = QUOTED;
		index++;
	}
	if (exp->need_expand == NO_EXPAND && exp->quote == QUOTED)
		(*to_check)->word = remove_quote((*to_check)->word, state);
	else
		return (EXPANSION_REQUIRED);
	return (NO_ACTION_REQUIRED);
}

/**
 * @nom: dollar_at_end
 * @brief: Vérifie si un symbole dollar se trouve à la fin d'une chaîne de caractères.
 *
 * @description:
 *   Parcourt une chaîne de caractères 'str' pour déterminer si le dernier caractère est un 
 *   symbole dollar ('$'). Cette vérification est utile dans le contexte de l'interprétation 
 *   des chaînes de caractères dans un shell, où le symbole dollar peut avoir une signification 
 *   spéciale pour l'expansion de variables.
 *
 * @param str: char *str, pointeur vers la chaîne de caractères à vérifier.
 *
 * @pourquoi:
 *   - Identification de Variables : Dans un shell, un dollar à la fin d'une chaîne peut indiquer 
 *     une variable non terminée ou une erreur de syntaxe.
 *   - Précision de l'Analyse : Permet de distinguer les cas où le symbole dollar est utilisé littéralement 
 *     ou comme partie d'une variable, influençant le traitement de la chaîne.
 *
 * @valeur_de_retour:
 *   Retourne CHAR_IS_DOLLAR si un dollar est trouvé à la fin, sinon CHAR_IS_NOT_DOLLAR.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'str' est NULL ou vide, la fonction retournera CHAR_IS_NOT_DOLLAR.
 *   - La fonction ne modifie pas la chaîne d'entrée.
 *
 * @exemples_d'utilisation:
 *   char *example1 = "text$";
 *   int result1 = dollar_at_end(example1); // Retourne CHAR_IS_DOLLAR
 *   
 *   char *example2 = "text";
 *   int result2 = dollar_at_end(example2); // Retourne CHAR_IS_NOT_DOLLAR
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
int	dollar_at_end(char *str)
{
	int	index;

	index = ZERO_INIT;
	while (str[index])
		index++;
	if (str[--index] == '$')
		return (CHAR_IS_DOLLAR);
	return (CHAR_IS_NOT_DOLLAR);
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
 * @dependances: Utilise 'is_dollr_quot_apstrph', 'dollar_at_end', 'need_expand_or_rm_quote', 
 *               et 'manage_expantion' pour diverses opérations d'expansion.
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
void	expand(t_quote *state, char **env, t_lexer *tmp)
{
	t_lexer		*save;
	t_expand	*exp;

	state = ft_malloc_with_tracking(sizeof(t_quote));
	if (!state)
		return ;
	reset_quoting_state(state);
	exp = ft_malloc_with_tracking(sizeof(t_expand));
	if (!exp)
		return ;
	while (tmp)
	{
		save = tmp;
		if (is_dollr_quot_apstrph(tmp->word))
		{
			tmp = tmp->next;
			reset_quoting_state(state);
			if (!dollar_at_end(save->word))
				continue ;
			if (need_expand_or_rm_quote(&save, state, exp))
				manage_expantion(&save, state, env, exp);
		}
		else
			tmp = tmp->next;
	}
}
