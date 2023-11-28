/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 12:50:48 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/28 16:33:27 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @fonction: next_dquote_len
 * @brève_description: Calcule la longueur d'un segment de commande jusqu'à la prochaine guillemet double (").
 * 
 * @param cmd: Chaîne de caractères représentant la commande à analyser.
 * @param index: Position de départ dans la chaîne 'cmd' pour commencer l'analyse.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'next_dquote_len' parcourt la chaîne 'cmd' à partir de 'index', cherchant la fin du segment
 *   actuel qui est délimité par une guillemet double. Elle met à jour l'index pour pointer après la fin de 
 *   ce segment. La fonction gère également l'état des guillemets et les caractères de séparation pour
 *   identifier correctement la fin du segment.
 *   
 *   Pourquoi cette analyse est-elle importante ?
 *   - Parsing correct des commandes : La détection précise des limites des segments de commande est essentielle 
 *     pour un interpréteur de commande pour comprendre et exécuter correctement les commandes de l'utilisateur.
 *   - Gestion des états de guillemets : Les guillemets doubles affectent la manière dont les commandes sont 
 *     interprétées, donc leur gestion correcte est cruciale.
 *
 * @valeur_de_retour: 
 *   Retourne l'index mis à jour après la fin du segment analysé.
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'cmd' est NULL ou si 'index' est hors de la portée de 'cmd', le comportement de la fonction est indéfini.
 *   - La fonction modifie l'état de 'state' qui peut affecter l'analyse des segments ultérieurs de la commande.
 *
 * @exemples_d'utilisation:
 *   char *cmd = "echo \"hello world\"";
 *   int index = 5; // Position de départ après "echo "
 *   t_quote state;
 *   int new_index = next_dquote_len(cmd, index, &state); // Résultat attendu : index après "world"
 *
 * @dépendances:
 *   - sep_word, end_word, update_quoting_state : Fonctions pour analyser les mots et l'état des guillemets.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Le caractère actuel (cmd[index]) est-il un guillemet double (") ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Incrementer  Continuer sans changement
 * index        de l'index
 *   |
 *   v
 * Parcourir la chaîne 'cmd' jusqu'à trouver un guillemet double ou la fin de la chaîne
 *   |
 *   v
 * Le caractère actuel est-il la fin de la chaîne ('\0') ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Mettre à jour l'état des guillemets avec update_quoting_state
 * index      et incrémenter l'index
 *   |
 *   v
 * Le caractère actuel (cmd[index]) est-il un séparateur de mot ?
 *  /       \
 * NON       OUI
 *  |         |
 *  |         |
 *  v         v
 * Continuer  Retourner l'index
 * la        actuel
 * parcours
 *   |
 *   v
 * Parcourir la chaîne 'cmd' en mettant à jour l'état des guillemets
 * et en incrémentant l'index jusqu'à atteindre la fin d'un mot
 *   |
 *   v
 * Retourner l'index actuel
 * Fin
 */
int	next_dquote_len(char *cmd, int index, t_quote *state)
{
	if (cmd[index] == '"')
		index++;
	while (cmd[index] && cmd[index] != '"')
		index++;
	if (cmd[index] == '\0')
		return (index);
	update_quoting_state(cmd[index], state);
	index++;
	if (cmd[index] && !sep_word(cmd[index]))
	{
		while (cmd[index] && !end_word(cmd, index, state))
		{
			update_quoting_state(cmd[index], state);
			index++;
		}
	}
	return (index);
}

/**
 * @fonction: update_quoting_state
 * @brève_description: Met à jour l'état des guillemets dans une analyse de chaîne de caractères.
 * 
 * @param c: Caractère courant à analyser.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'update_quoting_state' ajuste l'état des guillemets (simples et doubles) en fonction
 *   du caractère courant 'c' et de l'état précédent des guillemets. Elle gère les changements d'état 
 *   entre guillemets ouverts et fermés, et détermine si un guillemet simple est actuellement le 
 *   délimiteur actif.
 *   
 *   Pourquoi cette mise à jour est-elle nécessaire ?
 *   - Analyse précise : La gestion correcte des guillemets est cruciale pour interpréter correctement 
 *     les chaînes de caractères, en particulier dans les interpréteurs de commandes où les guillemets 
 *     changent la signification des caractères.
 *   - Flexibilité dans l'analyse : Cette fonction permet de traiter correctement les cas où les guillemets 
 *     s'ouvrent et se ferment, permettant une analyse flexible et précise des chaînes de caractères.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'state' est NULL, la fonction ne peut pas mettre à jour l'état des guillemets.
 *   - La fonction change l'état de 'state', ce qui affecte l'analyse des caractères subséquents dans la chaîne.
 *
 * @exemples_d'utilisation:
 *   t_quote state;
 *   char c = '\'';
 *   update_quoting_state(c, &state); // Met à jour l'état des guillemets pour le caractère '\''
 *
 * @dépendances: Aucune.
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
 *           définir 'sq_first' à 1
 *             |
 *             v
 *           Fin
 */
void	update_quoting_state(char c, t_quote *state)
{
	if (c == '\'')
	{
		if (state->is_quote == 0)
			state->is_quote = 1;
		else
		{
			state->sq_first = 0;
			state->is_quote = 0;
		}
	}
	else if (c == '"')
	{
		if (state->is_dquote == 0)
			state->is_dquote = 1;
		else
			state->is_dquote = 0;
	}
	if (state->is_quote == 1 && state->is_dquote == 0)
		state->sq_first = 1;
}

/**
 * @fonction: sep_word
 * @brève_description: Vérifie si un caractère est un séparateur de mot.
 * 
 * @param c: Caractère à évaluer.
 * 
 * @description_détaillée:
 *   La fonction 'sep_word' évalue si le caractère fourni 'c' est l'un des séparateurs de mot spécifiques, 
 *   tels que '<', '>', '|', ou un espace (' '). Ces séparateurs sont souvent utilisés dans les interpréteurs 
 *   de commande pour délimiter les commandes, les arguments et les redirections.
 *   
 *   Pourquoi identifier les séparateurs de mot ?
 *   - Analyse de commande : Les séparateurs de mot sont essentiels pour distinguer les différentes parties 
 *     d'une commande dans un interpréteur de commandes, permettant ainsi une analyse et une exécution correctes.
 *   - Clarté du parsing : Reconnaître ces caractères permet de segmenter correctement une chaîne de caractères 
 *     en mots ou commandes distincts.
 *
 * @valeur_de_retour: 
 *   Retourne 1 si 'c' est un séparateur de mot, 0 sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: Aucun. La fonction effectue une vérification sans état et sans effet de bord.
 *
 * @exemples_d'utilisation:
 *   char c = '|';
 *   int result = sep_word(c); // Résultat attendu : 1 (vrai)
 *
 * @dépendances: Aucune.
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
int	sep_word(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == ' ')
		return (1);
	return (0);
}

/**
 * @fonction: end_word
 * @brève_description: Détermine si le caractère actuel marque la fin d'un mot dans une chaîne de commande.
 * 
 * @param cmd: Chaîne de caractères représentant la commande.
 * @param i: Index du caractère actuel dans la chaîne 'cmd'.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'end_word' vérifie si le caractère à l'index 'i' dans la chaîne 'cmd' est un séparateur 
 *   de mot (comme défini par la fonction 'sep_word') et si ce caractère n'est pas à l'intérieur d'un 
 *   segment entre guillemets. Cette fonction est utilisée pour identifier les limites des mots ou 
 *   des commandes dans une chaîne de caractères.
 *   
 *   Pourquoi est-il important de détecter la fin d'un mot ?
 *   - Analyse correcte des commandes : Identifier les fins des mots permet de séparer correctement les 
 *     commandes et leurs arguments, ce qui est crucial pour l'interprétation et l'exécution des commandes.
 *   - Gestion des guillemets : La prise en compte de l'état des guillemets assure que les séparateurs 
 *     de mot à l'intérieur des guillemets ne sont pas traités comme des fins de mot.
 *
 * @valeur_de_retour: 
 *   Retourne 1 si le caractère actuel est un séparateur de mot et qu'il n'est pas à l'intérieur des guillemets, 0 sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'cmd' est NULL ou si 'i' est hors de la plage de 'cmd', le comportement est indéfini.
 *   - La fonction dépend de l'état de 'state' pour déterminer si les guillemets sont actifs.
 *
 * @exemples_d'utilisation:
 *   char *cmd = "echo \"hello world\"";
 *   t_quote state;
 *   int index = 4; // Position après 'echo'
 *   int result = end_word(cmd, index, &state); // Résultat attendu : 1 (vrai)
 *
 * @dépendances:
 *   - sep_word : Fonction pour identifier les séparateurs de mots.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier si le caractère actuel est un séparateur de mot, et si les guillemets ne sont pas actifs
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
int	end_word(char *cmd, int i, t_quote *state)
{
	if (sep_word(cmd[i]) && state->is_dquote == 0 && state->is_quote == 0)
		return (1);
	return (0);
}

/**
 * @fonction: len_darr
 * @brève_description: Calcule la longueur d'un tableau de chaînes de caractères.
 * 
 * @param arr: Pointeur vers un tableau de chaînes de caractères (tableau de pointeurs).
 * 
 * @description_détaillée:
 *   La fonction 'len_darr' parcourt un tableau de chaînes de caractères (un tableau doublement 
 *   indirect) et compte le nombre d'éléments jusqu'à ce qu'elle atteigne un élément NULL, qui 
 *   marque la fin du tableau. Cette fonction est souvent utilisée pour déterminer la taille de 
 *   tableaux dynamiques de chaînes, comme les arguments d'une commande.
 *   
 *   Pourquoi calculer la longueur de ce type de tableau ?
 *   - Gestion des données dynamiques : Permet de travailler avec des tableaux de chaînes dont la taille 
 *     n'est pas connue à l'avance, typique dans les environnements où les données varient dynamiquement.
 *   - Préparation à la manipulation : Connaître la taille du tableau est essentiel pour des opérations 
 *     comme l'itération, la copie, ou la libération de la mémoire.
 *
 * @valeur_de_retour: 
 *   Retourne le nombre de chaînes de caractères dans le tableau 'arr', excluant l'élément NULL final.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'arr' est NULL, la fonction retournera 0.
 *   - La fonction ne modifie pas le tableau 'arr' et n'a pas d'effet de bord.
 *
 * @exemples_d'utilisation:
 *   char *commandes[] = {"ls", "-l", "/home", NULL};
 *   int taille = len_darr(commandes); // Résultat attendu : 3
 *
 * @dépendances: Aucune.
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
 * Retourner la valeur de 'index' (longueur du tableau sans compter l'élément NULL)
 * Fin
 */
int	len_darr(char **arr)
{
	int	index;

	index = ZERO_INIT;
	while (arr[index])
		index++;
	return (index);
}
