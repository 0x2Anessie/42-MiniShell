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

