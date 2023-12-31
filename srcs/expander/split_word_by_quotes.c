#include "../../include/minishell.h"

/**
 * @nom: count_subwords_with_quote_states
 *
 * @description: Calcule le nombre de sous-mots dans un mot, en tenant compte
 *               des états de citation et de double citation.
 *
 * @parametres:
 *   - word: char *word, pointeur vers le mot à analyser.
 *   - state: t_quote *state, pointeur vers une structure représentant l'état des
 *            citations et des doubles citations.
 *
 * @fonctionnement:
 * La fonction initialise deux compteurs à zéro. Ensuite, elle parcourt le mot
 * caractère par caractère, mettant à jour les états de citation et de double
 * citation à chaque itération. Pour chaque sous-mot détecté (un mot est défini
 * comme un ensemble de caractères séparé par des espaces, à moins d'être entre
 * des citations), le compteur de sous-mots (j) est incrémenté. Cela permet de
 * déterminer combien de sous-mots distincts sont présents dans le mot, ce qui
 * est crucial pour les traitements ultérieurs, notamment dans le contexte
 * d'un shell.
 *
 * Importance de la distinction des sous-mots :
 * - Précision de l'analyse : Permet de traiter correctement les commandes
 * et arguments
 * dans un shell, en respectant la présence de citations et de doubles citations.
 * - Gestion correcte des espaces : Les espaces à l'intérieur des citations ne
 * sont pas considérés comme des séparateurs de mots, ce qui nécessite une
 * analyse plus fine.
 *
 * @valeur_retour: 
 * Renvoie le nombre total de sous-mots trouvés dans le mot.
 *
 * @erreurs: 
 * - Aucune erreur spécifique n'est retournée par cette fonction.
 *
 * @exemple_utilisation:
 * char *word = "commande \"argument avec espaces\"";
 * t_quote state = {0, 0};
 * int nombre_sous_mots = count_subwords_with_quote_states(word, &state);
 *
 * @dependances: 
 * - update_quoting_state pour mettre à jour l'état des citations.
 * - is_start_word_considering_quotes pour déterminer si un caractère
 * appartient à un nouveau sous-mot.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialisation de index et j à ZERO_INIT
 *     |
 *     v
 *   Initialisation des états de citation et de double citation à ZERO_INIT
 *     |
 *     v
 *   Parcourir word
 *     |
 *     v
 *   Mise à jour de l'état de citation avec update_quoting_state
 *     |
 *     v
 *   Est-on sur un nouveau mot ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Incrémenter  Continuer avec
 * j            le prochain caractère
 *     |
 *     v
 *   Fin de la chaîne word atteinte ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Retourner   Continuer l'itération
 * j
 *   Fin
 */
int	count_subwords_with_quote_states(char *word, t_quote *state)
{
	int		index;
	int		j;

	index = ZERO_INIT;
	j = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	while (word[index])
	{
		update_quoting_state(word[index], state);
		if (is_start_word_considering_quotes(word, index, state))
			j++;
		index++;
	}
	return (j);
}

/**
 * @nom: split_word_by_quotes
 *
 * @description:
 * Sépare un mot en plusieurs sous-mots en fonction des citations et des
 * doubles citations.
 *
 * @parametres:
 * - word: char *word, pointeur vers le mot à séparer.
 * - state: t_quote *state, pointeur vers une structure représentant l'état des
 * citations et des doubles citations.
 *
 * @fonctionnement:
 * La fonction commence par allouer de la mémoire pour un tableau de chaînes de
 * caractères. Elle initialise ensuite les index et les états des citations. En
 * parcourant chaque caractère du mot, elle met à jour l'état de citation à
 * chaque caractère et, si le caractère courant marque le début d'un sous-mot,
 * alloue de la mémoire pour ce sous-mot et l'ajoute au tableau. Ce processus
 * continue jusqu'à la fin du mot, après quoi la fonction ajoute un pointeur NULL
 * pour marquer la fin du tableau.
 * 
 * Importance de la gestion des citations :
 * - Distinguer les sous-mots : Dans un contexte de shell, par exemple, les
 * mots peuvent être séparés par des espaces, mais les espaces à l'intérieur
 * des citations ne doivent pas être considérés comme des séparateurs.
 * - Préserver l'intégrité des sous-mots : Les sous-mots à l'intérieur des
 * citations doivent être traités comme des unités indivisibles, même s'ils
 * contiennent des espaces ou d'autres caractères spéciaux.
 *
 * @valeur_retour: 
 * Renvoie un tableau de chaînes de caractères contenant les sous-mots. Renvoie
 * NULL si l'allocation échoue.
 *
 * @erreurs: 
 * - Échec de l'allocation de mémoire (retourne NULL).
 *
 * @exemple_utilisation:
 * char *word = "exemple \"de mot\" séparé";
 * t_quote state = {0, 0};
 * char **mots = split_word_by_quotes(word, &state);
 *
 * @dependances: 
 * - ft_malloc_with_tracking pour l'allocation de mémoire.
 * - count_subwords_with_quote_states pour déterminer la taille du tableau.
 * - alloc_copy_word_excluding_quot_with_tracking pour allouer chaque sous-mot.
 * - update_quoting_state pour mettre à jour l'état des citations.
 * - is_start_word_considering_quotes pour vérifier si un caractère appartient
 * à un nouveau sous-mot.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Allocation de mémoire pour arr
 *     |
 *     v
 *   Allocation réussie ?
 *  /        \
 * NON       OUI
 *  |         \
 *  |          \
 *  v           v
 * Retourner   Initialiser index, j, et l'état des citations
 * NULL
 *     |
 *     v
 *   Parcourir word
 *     |
 *     v
 *   Mise à jour de l'état de citation
 *     |
 *     v
 *   Est-on sur un nouveau mot ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Allouer un   Continuer avec
 * nouveau mot  le prochain caractère
 * dans arr
 *     |
 *     v
 *   Arriver à la fin de word ?
 *  /        \
 * NON       OUI
 *  |         \
 *  |          \
 *  v           v
 * Continuer   Marquer la fin du tableau avec NULL
 * l'itération
 *     |
 *     v
 *   Retourner arr
 *   Fin
 */
char	**split_word_by_quotes(t_data *data, char *word, t_quote *state)
{
	int		index;
	int		j;
	char	**arr;

	arr = \
	ft_malloc_with_tracking(\
	data, sizeof(char *) * (count_subwords_with_quote_states(\
	word, state) + sizeof('\0')));
	if (!arr)
		return (NULL);
	index = ZERO_INIT;
	j = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	while (word[index])
	{
		update_quoting_state(word[index], state);
		if (is_start_word_considering_quotes(word, index, state))
			arr[j++] = \
			alloc_copy_word_excluding_quot_with_tracking(\
			&word[index], state, data);
		index++;
	}
	arr[j] = NULL;
	return (arr);
}

/**
 * @fonction: next_squote
 * @breve_description: Trouve la position de la prochaine guillemet simple
 * dans une chaîne de commande.
 * 
 * @param cmd: Chaîne de caractères représentant la commande à analyser.
 * 
 * @description_detaillee:
 * La fonction 'next_squote' parcourt la chaîne 'cmd' pour trouver la position
 * de la prochaine guillemet simple ('). Elle commence à l'index zéro et
 * vérifie les conditions suivantes :
 * - Si le premier caractère est un guillemet simple suivi immédiatement par
 * un autre guillemet simple, elle retourne l'index actuel.
 * - Sinon, elle parcourt la chaîne jusqu'à trouver un guillemet simple ou
 * atteindre la fin de la chaîne.
 *   
 * Pourquoi identifier la position de la guillemet simple ?
 * - Parsing de commande : La détection des guillemets simples est essentielle
 * pour interpréter correctement les chaînes de caractères dans un interpréteur
 * de commande, notamment pour gérer les chaînes littérales.
 * - Gestion des chaînes : Permet de traiter ou de manipuler correctement des
 * segments spécifiques d'une commande.
 *
 * @valeur_de_retour: 
 * Retourne l'index de la position de la prochaine guillemet simple dans 'cmd'.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'cmd' est NULL, le comportement de la fonction est indéfini.
 *
 * @exemples_utilisation:
 * char *cmd = "echo 'Hello, world!'";
 * int position = next_squote(cmd); // Résultat attendu : position du premier
 * guillemet simple après "echo "
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser 'index' à ZERO_INIT
 *   |
 *   v
 * Le premier caractère de 'cmd' est-il un guillemet simple ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Le deuxième caractère  Incrementer 'index'
 * est aussi un guillemet  et continuer
 * simple ?                la recherche
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Parcourir 'cmd' jusqu'à
 * l'index    trouver le prochain guillemet simple
 * actuel     ou la fin de la chaîne
 *              |
 *              v
 *            Retourner l'index où le guillemet simple a été trouvé
 * Fin
 */

// int	next_squote(char *cmd)
// {
// 	int	index;

// 	index = ZERO_INIT;
// 	if (cmd[index] == '\'' && cmd[index + FORWARD_OFFSET] == '\'')
// 		return (index);
// 	else if (cmd[index] == '\'')
// 		index++;
// 	while (cmd[index] && cmd[index + FORWARD_OFFSET] != '\'')
// 		index++;
// 	return (index);
// }

/**
 * @fonction: next_dquote
 * @breve_description: Trouve la position de la prochaine guillemet double dans
 * une chaîne de commande.
 * 
 * @param cmd: Chaîne de caractères représentant la commande à analyser.
 * 
 * @description_detaillee:
 * La fonction 'next_dquote' parcourt la chaîne 'cmd' pour trouver la position
 * de la prochaine guillemet double ("). Elle commence à l'index zéro et
 * vérifie les conditions suivantes :
 * - Si le premier caractère est un guillemet double suivi immédiatement par
 * un autre guillemet double, elle retourne l'index actuel.
 * - Sinon, elle parcourt la chaîne jusqu'à trouver un guillemet double ou
 * atteindre la fin de la chaîne.
 *   
 * Pourquoi identifier la position de la guillemet double ?
 * - Parsing de commande : La détection des guillemets doubles est essentielle
 * pour interpréter correctement les chaînes de caractères dans un interpréteur
 * de commande, notamment pour gérer les chaînes littérales.
 * - Gestion des chaînes : Permet de traiter ou de manipuler correctement des
 * segments spécifiques d'une commande.
 *
 * @valeur_de_retour: 
 * Retourne l'index de la position de la prochaine guillemet double dans 'cmd'.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'cmd' est NULL, le comportement de la fonction est indéfini.
 *
 * @exemples_utilisation:
 * char *cmd = "echo \"Hello, world!\"";
 * int position = next_dquote(cmd); // Résultat attendu : position du premier
 * guillemet double après "echo "
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser 'index' à ZERO_INIT
 *   |
 *   v
 * Le premier caractère de 'cmd' est-il un guillemet double ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Le deuxième caractère  Incrementer 'index'
 * est aussi un guillemet  et continuer
 * double ?                la recherche
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Parcourir 'cmd' jusqu'à
 * l'index    trouver le prochain guillemet double
 * actuel     ou la fin de la chaîne
 *              |
 *              v
 *            Retourner l'index où le guillemet double a été trouvé
 * Fin
 */
// int	next_dquote(char *cmd)
// {
// 	int	index;

// 	index = ZERO_INIT;
// 	if (cmd[index] == '"' && cmd[index + FORWARD_OFFSET] == '"')
// 		return (index);
// 	else if (cmd[index] == '"')
// 		index++;
// 	while (cmd[index] && cmd[index + FORWARD_OFFSET] != '"')
// 		index++;
// 	return (index);
// }

/**
 * @fonction: next_squote_len
 * @breve_description: Calcule la longueur d'un segment de commande jusqu'à la
 * prochaine guillemet simple (').
 * 
 * @param cmd: Chaîne de caractères représentant la commande à analyser.
 * @param index: Position de départ dans la chaîne 'cmd' pour commencer
 * l'analyse.
 * @param state: Pointeur sur une structure t_quote représentant l'état
 * actuel des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'next_squote_len' parcourt la chaîne 'cmd' à partir de 'index',
 * cherchant la fin du segment actuel qui est délimité par une guillemet
 * simple. Elle met à jour l'index pour pointer après la fin de ce segment. La
 * fonction gère également l'état des guillemets et les caractères de
 * séparation pour identifier correctement la fin du segment.
 *   
 * Pourquoi cette analyse est-elle importante ?
 * - Parsing correct des commandes : La détection précise des limites des
 * segments de commande est essentielle pour un interpréteur de commande pour
 * comprendre et exécuter correctement les commandes de l'utilisateur.
 * - Gestion des états de guillemets : Les guillemets simples affectent la
 * manière dont les commandes sont interprétées, donc leur gestion correcte est
 * cruciale.
 *
 * @valeur_de_retour: 
 * Retourne l'index mis à jour après la fin du segment analysé.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'cmd' est NULL ou si 'index' est hors de la portée de 'cmd', le
 * comportement de la fonction est indéfini.
 * - La fonction modifie l'état de 'state' qui peut affecter l'analyse des
 * segments ultérieurs de la commande.
 *
 * @exemples_utilisation:
 * char *cmd = "echo 'hello world'";
 * int index = 5; // Position de départ après "echo "
 * t_quote state;
 * int new_index = next_squote_len(cmd, index, &state);
 * ---------- >Résultat attendu : index après "world"
 *
 * @dependances:
 * - is_char_cmd_separator, is_end_of_command_word, update_quoting_state :
 * Fonctions pour analyser les mots et l'état des guillemets.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Le caractère actuel (cmd[index]) est-il un guillemet simple (') ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Incrementer  Continuer sans changement
 * index        de l'index
 *   |
 *   v
 * Parcourir la chaîne 'cmd'
 * jusqu'à trouver un guillemet simple 
 *        ou 
 * la fin de la chaîne
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
// int	next_squote_len(char *cmd, int index, t_quote *state)
// {
// 	if (cmd[index] == '\'')
// 		index++;
// 	while (cmd[index] && cmd[index] != '\'')
// 		index++;
// 	if (cmd[index] == '\0')
// 		return (index);
// 	update_quoting_state(cmd[index], state);
// 	index++;
// 	if (cmd[index] && !is_char_cmd_separator(cmd[index]))
// 	{
// 		while (cmd[index] && !is_end_of_command_word(cmd, index, state))
// 		{
// 			update_quoting_state(cmd[index], state);
// 			index++;
// 		}
// 	}
// 	return (index);
// }
