/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management_and_expander_utils.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 17:54:00 by raveriss          #+#    #+#             */
/*   Updated: 2024/01/16 19:20:22 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @fonction: update_quoting_state
 * @breve_description: Met à jour l'état des guillemets dans une analyse de
 * chaîne de caractères.
 * 
 * @param c: Caractère courant à analyser.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel
 * des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'update_quoting_state' ajuste l'état des guillemets (simples et
 * doubles) en fonction du caractère courant 'c' et de l'état précédent des
 * guillemets. Elle gère les changements d'état entre guillemets ouverts et
 * fermés, et détermine si un guillemet simple est actuellement le délimiteur
 * actif.
 *   
 * Pourquoi cette mise à jour est-elle nécessaire ?
 * - Analyse précise : La gestion correcte des guillemets est cruciale pour
 * interpréter correctement les chaînes de caractères, en particulier dans les
 * interpréteurs de commandes où les guillemets changent la signification des
 * caractères.
 * - Flexibilité dans l'analyse : Cette fonction permet de traiter correctement
 * les cas où les guillemets s'ouvrent et se ferment, permettant une analyse
 * flexible et précise des chaînes de caractères.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'state' est NULL, la fonction ne peut pas mettre à jour l'état des
 * guillemets.
 * - La fonction change l'état de 'state', ce qui affecte l'analyse des
 * caractères subséquents dans la chaîne.
 *
 * @exemples_utilisation:
 * t_quote state;
 * char c = '\'';
 * update_quoting_state(c, &state);
 * -------> Met à jour l'état des guillemets pour le caractère '\''
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Analyser le caractère 'c'
 *     |
 *     v
 *   'c' est-il un guillemet simple (') ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Mettre à   'c' est-il un guillemet double (") ?
 * jour l'état /       \
 * des        OUI      NON
 * guillemets  |         |
 * simples     |         |
 *             v         v
 *           Mettre à   Aucune action pour les autres caractères
 *           jour l'état
 *           des guillemets
 *           doubles
 *             |
 *             v
 *           Si guillemet simple actif et double inactif,
 *           définir 'singl_quot_start_status' à 1
 *             |
 *             v
 *           Fin
 */
void	update_quoting_state(char c, t_quote *state)
{
	if (c == '\'')
	{
		if (is_single_quote_closed(state))
			state->singl_quot_status = SIMPL_QUOT_OPEN;
		else
		{
			state->singl_quot_start_status = SINGLE_QUOTE_NOT_STARTED;
			state->singl_quot_status = SIMPL_QUOT_CLOS;
		}
	}
	else if (c == '"')
	{
		if (is_doubl_quote_closed(state))
			state->doubl_quot_status = DOUBL_QUOT_OPEN;
		else
			state->doubl_quot_status = DOUBL_QUOT_CLOS;
	}
	if (is_only_single_quote_on(state))
		state->singl_quot_start_status = SINGLE_QUOTE_STARTED;
}

/**
 * @fonction: is_char_cmd_separator
 * @breve_description: Vérifie si un caractère est un séparateur de mot.
 * 
 * @param c: Caractère à évaluer.
 * 
 * @description_detaillee:
 * La fonction 'is_char_cmd_separator' évalue si le caractère fourni 'c' est
 * l'un des séparateurs de mot spécifiques, tels que '<', '>', '|', ou un
 * espace (' '). Ces séparateurs sont souvent utilisés dans les interpréteurs
 * de commande pour délimiter les commandes, les arguments et les redirections.
 *   
 * Pourquoi identifier les séparateurs de mot ?
 * - Analyse de commande : Les séparateurs de mot sont essentiels pour
 * distinguer les différentes parties d'une commande dans un interpréteur de
 * commandes, permettant ainsi une analyse et une exécution correctes.
 * - Clarté du parsing : Reconnaître ces caractères permet de segmenter
 * correctement une chaîne de caractères en mots ou commandes distincts.
 *
 * @valeur_de_retour: 
 * Retourne 1 si 'c' est un séparateur de mot, 0 sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: Aucun. La fonction effectue une
 * vérification sans état et sans effet de bord.
 *
 * @exemples_utilisation:
 * char c = '|';
 * int result = is_char_cmd_separator(c);
 * -------> Résultat attendu : 1 (vrai)
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier si 'c' est l'un des caractères suivants: '<', '>', '|', ' '
 *     |
 *     v
 *   'c' est-il un séparateur de mot ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Retourner 0
 * 1
 */
int	is_char_cmd_separator(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == ' ')
		return (IS_SEPARATOR);
	return (NOT_SEPARATOR);
}

bool	is_cmd_separator_and_quotes_off(char c, t_quote *state)
{
	return (is_char_cmd_separator(c) && is_both_quotes_off(state));
}

/**
 * @fonction: is_end_of_command_word
 * @breve_description: Détermine si le caractère actuel marque la fin d'un mot
 * dans une chaîne de commande.
 * 
 * @param cmd: Chaîne de caractères représentant la commande.
 * @param i: Index du caractère actuel dans la chaîne 'cmd'.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel
 * des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'is_end_of_command_word' vérifie si le caractère à l'index 'i'
 * dans la chaîne 'cmd' est un séparateur de mot (comme défini par la fonction
 * 'is_char_cmd_separator') et si ce caractère n'est pas à l'intérieur d'un
 * segment entre guillemets. Cette fonction est utilisée pour identifier les
 * limites des mots ou des commandes dans une chaîne de caractères.
 *   
 * Pourquoi est-il important de détecter la fin d'un mot ?
 * - Analyse correcte des commandes : Identifier les fins des mots permet de
 * séparer correctement les commandes et leurs arguments, ce qui est crucial
 * pour l'interprétation et l'exécution des commandes.
 * - Gestion des guillemets : La prise en compte de l'état des guillemets
 * assure que les séparateurs de mot à l'intérieur des guillemets ne sont pas
 * traités comme des fins de mot.
 *
 * @valeur_de_retour: 
 * Retourne 1 si le caractère actuel est un séparateur de mot et qu'il n'est
 * pas à l'intérieur des guillemets, 0 sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'cmd' est NULL ou si 'i' est hors de la plage de 'cmd', le comportement
 * est indéfini.
 * - La fonction dépend de l'état de 'state' pour déterminer si les guillemets
 * sont actifs.
 *
 * @exemples_utilisation:
 * char *cmd = "echo \"hello world\"";
 * t_quote state;
 * int index = 4; // Position après 'echo'
 * int result = is_end_of_command_word(cmd, index, &state);
 * -------> Résultat attendu : 1 (vrai)
 *
 * @dependances:
 * - is_char_cmd_separator : Fonction pour identifier les séparateurs de mots.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier
 * si le caractère actuel
 * est un séparateur de mot, et 
 * si les guillemets ne sont pas actifs
 *     |
 *     v
 *   Le caractère actuel est-il un séparateur de mot hors guillemets ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Retourner 0
 * 1
 */
int	is_end_of_command_word(char *cmd, int i, t_quote *state)
{
	if (is_cmd_separator_and_quotes_off(cmd[i], state))
		return (TRUE);
	return (FALSE);
}

/**
 * @fonction: count_string_array_elements
 * @breve_description: Calcule la longueur d'un tableau de chaînes de
 * caractères.
 * 
 * @param arr: Pointeur vers un tableau de chaînes de caractères
 * (tableau de pointeurs).
 * 
 * @description_detaillee:
 * La fonction 'count_string_array_elements' parcourt un tableau de chaînes de
 * caractères (un tableau doublement indirect) et compte le nombre d'éléments
 * jusqu'à ce qu'elle atteigne un élément NULL, qui marque la fin du tableau.
 * Cette fonction est souvent utilisée pour déterminer la taille de tableaux
 * dynamiques de chaînes, comme les arguments d'une commande.
 *   
 * Pourquoi calculer la longueur de ce type de tableau ?
 * - Gestion des données dynamiques : Permet de travailler avec des tableaux de
 * chaînes dont la taille n'est pas connue à l'avance, typique dans les
 * environnements où les données varient dynamiquement.
 * - Préparation à la manipulation : Connaître la taille du tableau est
 * essentiel pour des opérations comme l'itération, la copie, ou la libération
 * de la mémoire.
 *
 * @valeur_de_retour: 
 * Retourne le nombre de chaînes de caractères dans le tableau 'arr', excluant
 * l'élément NULL final.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'arr' est NULL, la fonction retournera 0.
 * - La fonction ne modifie pas le tableau 'arr' et n'a pas d'effet de bord.
 *
 * @exemples_utilisation:
 * char *commandes[] = {"ls", "-l", "/home", NULL};
 * int taille = count_string_array_elements(commandes);
 * -------> Résultat attendu : 3
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser 'index' à ZERO_INIT (zéro)
 *   |
 *   v
 * Entrer dans une boucle pour parcourir le tableau 'arr'
 *   |
 *   v
 * Vérifier si l'élément courant (arr[index]) est NULL
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Sortir de  Continuer le parcours : incrémenter 'index'
 * la boucle  et retourner à la vérification de l'élément courant
 *   |
 *   v
 * Retourner 
 * la valeur de 'index' 
 * (longueur du tableau sans compter l'élément NULL)
 * Fin
 */
int	count_string_array_elements(char **arr)
{
	int	index;

	index = ZERO_INIT;
	while (arr[index])
		index++;
	return (index);
}
