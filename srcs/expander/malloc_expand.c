#include "../../include/minishell.h"

/**
 * @nom: malloc_copy_string_excluding_inactive_quotes
 * @breve_description: Alloue de la mémoire pour une nouvelle chaîne sans
 * guillemets.
 * 
 * @param len: int len, longueur de la nouvelle chaîne à allouer.
 * @param old: char *old, pointeur vers l'ancienne chaîne contenant
 * potentiellement des guillemets.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'malloc_copy_string_excluding_inactive_quotes' alloue de la
 * mémoire pour une nouvelle chaîne de caractères qui est destinée à contenir
 * une copie de la chaîne 'old' sans les guillemets. Elle utilise la longueur
 * 'len' pour déterminer la taille nécessaire de la nouvelle chaîne, en
 * incluant un espace pour le caractère nul de terminaison. Après l'allocation,
 * elle appelle 'copy_string_excluding_quotes' pour remplir la nouvelle chaîne.
 *   
 * Pourquoi allouer une nouvelle chaîne sans guillemets ?
 * - Création de Chaînes Normalisées : Permet d'obtenir une version de la
 * chaîne sans les guillemets qui peuvent être nécessaires pour le traitement
 * ultérieur ou l'analyse syntaxique.
 * - Séparation de la Mémoire : Sépare la mémoire de la nouvelle chaîne de
 * celle de l'ancienne, permettant des modifications sans affecter la chaîne
 * d'origine.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur vers la nouvelle chaîne allouée ou NULL en cas d'échec
 * d'allocation.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si l'allocation échoue, la fonction retourne NULL.
 * - La fonction alloue de la mémoire qui doit être libérée par l'appelant.
 *
 * @exemples_utilisation:
 * char *texte = "\"Ceci est un test\"";
 * t_quote state = {0, 0};
 * -------> singl_quot_status = 0, doubl_quot_status = 0
 * int longueur = calculer_longueur_sans_guillemets(texte, &state);
 * char *resultat =
 * 					malloc_copy_string_excluding_inactive_quotes(
 * 						longueur, texte, &state);
 *
 * @dependances: 
 * - ft_malloc_with_tracking pour l'allocation de mémoire.
 * - copy_string_excluding_quotes pour remplir la nouvelle chaîne.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Allouer de la mémoire
 * pour la nouvelle chaîne 'nw' (taille 'len' + '\0')
 *   |
 *   v
 *   - Vérification de la réussite de l'allocation
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Appeler copy_string_excluding_quotes   - Retourner NULL
 *   - Retourner le pointeur 'nw'
 *   |
 *   v
 *   Fin
 */
char	*malloc_copy_string_excluding_inactive_quotes(\
int len, char *old, t_quote *state, t_data *data)
{
	char	*nw;

	nw = ft_malloc_with_tracking(data, sizeof(char) * (len + sizeof('\0')));
	if (!nw)
		return (NULL);
	return (copy_string_excluding_quotes(old, nw, state));
}

/**
 * @nom: create_cleaned_str_excluding_inactive_quots
 * @breve_description: Supprime les guillemets d'une chaîne de caractères en
 * tenant compte de l'état des guillemets.
 * 
 * @param old: char *old, pointeur vers la chaîne de caractères source
 * contenant les guillemets.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'create_cleaned_str_excluding_inactive_quots' analyse la chaîne
 * 'old' et crée une nouvelle chaîne de caractères où tous les guillemets
 * valides (non échappés ou non inclus dans d'autres guillemets) sont
 * supprimés. Elle utilise l'état des guillemets ('state') pour déterminer si
 * un guillemet doit être ignoré ou conservé. Cette fonction est utile pour
 * traiter des chaînes qui nécessitent l'élimination des guillemets pour des
 * opérations ultérieures, comme l'exécution de commandes.
 *   
 * Pourquoi supprimer les guillemets des chaînes ?
 * - Normalisation des Données : La suppression des guillemets permet d'obtenir
 * une chaîne normalisée pour le traitement ultérieur, en éliminant les
 * caractères qui pourraient être interprétés de manière spéciale dans certains
 * contextes.
 * - Préparation à l'Exécution : Dans les shells et les langages de script, les
 * guillemets sont souvent utilisés pour grouper des mots comme une seule
 * entité. Leur suppression prépare la chaîne pour une exécution ou une
 * analyse ultérieure où les guillemets ne sont plus nécessaires.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur vers la nouvelle chaîne de caractères sans guillemets.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'old' est NULL, la fonction pourrait se comporter de manière
 * imprévisible.
 * - La fonction alloue de la mémoire pour la nouvelle chaîne, qui doit être
 * libérée par l'appelant.
 *
 * @exemples_utilisation:
 * char *texte = "\"Ceci est un test\"";
 * t_quote state = {0, 0};
 * -------> singl_quot_status = 0, doubl_quot_status = 0
 * char *resultat = create_cleaned_str_excluding_inactive_quots(texte, &state);
 * -------> Résultat attendu : "Ceci est un test"
 *
 * @dependances: 
 * - update_quoting_state pour mettre à jour l'état des guillemets.
 * - malloc_copy_string_excluding_inactive_quotes pour allouer la nouvelle
 * chaîne sans guillemets.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser 'i' à -1 et 'len' à ZERO_INIT
 *   - Réinitialiser l'état des guillemets dans 'state'
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'old'
 *   |
 *   v
 *   - Incrémenter 'i'
 *   - Mettre à jour l'état des guillemets (update_quoting_state)
 *   |
 *   v
 *   - Vérifier si le caractère courant est un guillemet et gérer l'état
 *   /       \
 *  OUI      NON
 *   |         |
 *   |         v
 *   |------> - Vérifier si le caractère courant doit être inclus
 *   |           (non guillemet ou état actif)
 *   |             /                \
 *   |            OUI              NON
 *   |             |                |
 *   |             v                |
 *   |------> - Incrémenter 'len'   |
 *   |         |                    |
 *   \---------|--------------------/
 *             |
 *             v
 *   - Vérifier si la fin de 'old' est atteinte
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         |
 *   - Allouer une nouvelle chaîne sans guillemets
 *     (malloc_copy_string_excluding_inactive_quotes) avec 'len'
 *   - Retourner la nouvelle chaîne   |
 *   |                                |
 *   \--------------------------------/
 *   Fin
 */
char	*create_cleaned_str_excluding_inactive_quots(\
char *old, t_quote *state, t_data *data)
{
	int		i;
	int		len;

	i = -1;
	len = ZERO_INIT;
	state->singl_quot_status = 0;
	state->doubl_quot_status = 0;
	while (old[++i])
	{
		update_quoting_state(old[i], state);
		if (is_both_quotes_on(state))
			len++;
		if (old[i] == '"' && is_single_quote_closed(state))/*         ---> condition non intelligible --> fonction         */
			continue ;
		if (old[i] == '\'' && is_doubl_quote_closed(state))/*         ---> condition non intelligible --> fonction         */
			continue ;
		len++;
	}
	return (\
	malloc_copy_string_excluding_inactive_quotes(len, old, state, data));
}

/**
 * @nom: is_start_word_considering_quotes
 * @breve_description: Détermine si le caractère à l'indice spécifié marque le
 * début d'un mot.
 * 
 * @param word: char *word, pointeur vers la chaîne de caractères à analyser.
 * @param index: int index, l'indice du caractère à évaluer dans la chaîne.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'is_start_word_considering_quotes' vérifie si le caractère situé
 * à 'word[index]' est le début d'un mot. Un mot est défini comme une séquence
 * de caractères non-espaces qui suit un espace ou qui se trouve au début de la
 * chaîne. La fonction tient également compte de l'état des guillemets pour
 * déterminer si un guillemet ouvre ou non un mot valide.
 *   
 * Pourquoi est-il important de vérifier le début d'un mot ?
 * - Identification des Mots : Permet de distinguer les mots individuels dans
 * une chaîne, ce qui est crucial pour l'analyse et l'interprétation des
 * commandes ou des instructions.
 * - Respect de la Syntaxe : En tenant compte des guillemets, la fonction
 * assure que les mots sont correctement identifiés même dans des contextes où
 * la syntaxe est importante, comme dans les shells ou les langages de script.
 *
 * @valeur_de_retour: 
 * Retourne 1 (vrai) si le caractère à 'word[index]' est le début d'un mot,
 * sinon 0 (faux).
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'word' est NULL ou si 'index' dépasse la longueur de 'word', le
 * comportement est indéfini.
 * - La fonction ne modifie pas 'word' ou 'state'.
 *
 * @exemples_utilisation:
 * char *phrase = "Ceci est un test";
 * t_quote state = {0, 0}; // singl_quot_status = 0, doubl_quot_status = 0
 * int estMot = is_start_word_considering_quotes(phrase, 0, &state);
 * -------> Résultat attendu : 1 (vrai)
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Vérifier
 * si 'word[index]' n'est pas un espace et est 
 * suivi d'un espace ou en début de ligne
 *  /                    \
 * OUI                   NON
 *  |                     |
 *  v                     |
 * Vérifier si            |
 * 'word[index]'          |
 * est un guillemet       v
 * et gérer l'état        Retourner 0 (faux)
 *  /            \
 * OUI           NON
 *  |              \       
 *  v               ------\      
 * Vérifier si le         |
 * caractère suivant      v
 * n'est pas un espace    Retourner 1 (vrai)
 *  /             \
 * OUI            NON ---\
 *  |                    |
 *  |                    |
 *  v                    v
 * Retourner 1 (vrai)    Retourner 0 (faux)
 *   |
 *   v
 * Fin
 */
int	is_start_word_considering_quotes(char *word, int index, t_quote *state)
{
	if (word[index] != ' ' && (index == 0 || word[index - 1] == ' '))/*         ---> condition non intelligible --> fonction         */
	{
		if (is_doubl_quote_closed(state) \
		&& (word[index] == '"' || word[index] == '\''))/*         ---> condition non intelligible --> fonction         */
		{
			if (word[index + 1])/*         ---> condition non intelligible --> fonction         */
				if (word[index + 1] != ' ')/*         ---> condition non intelligible --> fonction         */
					return (1);
		}
		else
			return (1);
	}
	return (0);
}

/**
 * @nom: copy_alloc_memory_excluding_inactive_quot
 * @breve_description: Copie un mot depuis une chaîne en tenant compte de
 * l'état des guillemets.
 * 
 * @param word: char *word, pointeur vers la chaîne source contenant le mot à
 * copier.
 * @param state: t_quote *state, pointeur vers une structure représentant
 * l'état des guillemets.
 * @param str: char *str, pointeur vers la chaîne de destination où le mot
 * sera copié.
 * 
 * @description_detaillee:
 * La fonction 'copy_alloc_memory_excluding_inactive_quot' parcourt la chaîne
 * 'word' et copie chaque caractère dans 'str' jusqu'à rencontrer un espace ou
 * la fin de la chaîne, en tenant compte des guillemets. Elle saute les 
 * guillemets si l'état correspondant ('singl_quot_status' ou
 * 'doubl_quot_status') est inactif, permettant ainsi de copier uniquement le
 * contenu pertinent du mot.
 *   
 * Pourquoi gérer les guillemets lors de la copie ?
 * - Respect des Règles de Syntaxe : Les guillemets peuvent changer le sens
 * d'un mot dans un shell ou un langage de script. En les gérant correctement,
 * on assure que les mots sont interprétés comme ils le devraient.
 * - Fiabilité de la Copie : Assure que seuls les caractères significatifs du
 * mot sont copiés, en excluant les guillemets qui pourraient autrement
 * altérer la signification ou l'utilisation du mot.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur vers la chaîne 'str', qui contient maintenant le mot
 * copié.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'word' ou 'str' est NULL, le comportement de la fonction est indéfini.
 * - La fonction modifie le contenu de 'str'.
 *
 * @exemples_utilisation:
 * char source[] = "\"exemple\"";
 * t_quote state = {1, 0};
 * -------> singl_quot_status = 1, doubl_quot_status = 0
 * char destination[10];
 * copy_alloc_memory_excluding_inactive_quot(source, &state, destination);
 * -------> Résultat attendu dans destination : exemple
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser 'i' et 'j' à ZERO_INIT (zéro)
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'word'
 *   |
 *   v
 *   - Le caractère courant
 *     'word[i]' est-il un espace ou la fin de la chaîne ?
 *   /                     \
 *  OUI                    NON
 *   |                      |
 *   v                      v
 *   - Terminer la boucle   - Le caractère courant
 *   - Passer à la suite      est-il un guillemet (simple ou double) ?
 *                            /       \
 *                          OUI       NON
 *                           |         |
 *   /-----------------------/         v
 *   |                                 - Vérifier 
 *   |                                 |  l'état des guillemets 
 *   |                                 |  (state->singl_quot_status, 
 *   |                                 v  state->doubl_quot_status)
 *   |                                 - Incrémenter 'i'
 *   |                                 v  pour sauter le guillemet
 *   |                                 - Retourner 
 *   |                                 |  à la vérification 
 *   |                                 |  du caractère courant
 *   |                                 |
 *   |---------------------------------/
 *   v
 *   - Copier le caractère courant de 'word' dans 'str[j]' et incrémenter 'j'
 *   - Incrémenter 'i' pour passer au caractère suivant
 *   - Retourner à la vérification du caractère courant
 *   |
 *   v
 *   - Ajouter le caractère nul '\0' à la fin de 'str'
 *   |
 *   v
 *   - Retourner 'str'
 *   Fin
 */
char	*copy_alloc_memory_excluding_inactive_quot(\
char *word, t_quote *state, char *str)
{
	int		i;
	int		j;

	i = ZERO_INIT;
	j = ZERO_INIT;
	while (word[i] && word[i] != ' ')/*         ---> condition non intelligible --> fonction         */
	{
		if ((word[i] == '"' && is_single_quote_closed(state)) \
		|| (word[i] == '\'' && is_doubl_quote_closed(state)))/*     ---> condition trop longue --> fonction     */
			i++;
		else
		{
			str[j++] = word[i];
			i++;
		}
	}
	str[j] = '\0';
	return (str);
}

/**
 * @nom: alloc_copy_word_excluding_quot_with_tracking
 *
 * @description: Alloue de la mémoire pour une copie d'un mot, en tenant
 * compte des états de citation et de double citation.
 *
 * @parametres:
 * - word: char *word, pointeur vers le mot à copier.
 * - state: t_quote *state, pointeur vers une structure représentant l'état des
 * citations et des doubles citations.
 *
 * @fonctionnement: 
 * La fonction parcourt chaque caractère du mot jusqu'à rencontrer un espace,
 * tout en vérifiant les états de citation et de double citation. Elle calcule
 * la taille nécessaire pour la nouvelle chaîne, alloue de la mémoire pour
 * celle-ci et retourne un pointeur vers la nouvelle chaîne allouée.
 * 
 * Pourquoi vérifier les états de citation ?
 * - Gérer correctement les citations et doubles citations : 
 * Les citations (') et les doubles citations (") peuvent changer le sens d'un
 * mot dans certains contextes (comme les commandes shell). Il est important de
 * les traiter correctement pour assurer que le mot est interprété comme prévu.
 * - Ignorer les espaces dans les citations : 
 * Les espaces à l'intérieur des citations ne doivent pas être considérés comme
 * la fin du mot.
 *
 * @valeur_retour: 
 * Renvoie un pointeur vers la nouvelle chaîne allouée. Renvoie NULL si
 * l'allocation échoue.
 *
 * @erreurs: 
 * - Échec de l'allocation de mémoire (retourne NULL).
 *
 * @exemple_utilisation:
 * char *word = "exemple";
 * t_quote state = {0, 0};
 * char *copied_word =
 *                 alloc_copy_word_excluding_quot_with_tracking(word, &state);
 *
 * @dependances: 
 * - ft_malloc_with_tracking pour l'allocation de mémoire.
 * - copy_alloc_memory_excluding_inactive_quot pour la copie du mot.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialisation de i et j à ZERO_INIT
 *     |
 *     v
 *   Parcourir 'word' jusqu'à ' ' ou fin du mot
 *     |
 *     v
 *   Caractère est une citation ou double citation non active ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Incrémenter   Incrémenter i et j
 * seulement i
 *     |
 *     v
 *   Allouer de la mémoire pour la nouvelle chaîne
 *     |
 *     v
 *   Allocation réussie ?
 *  /        \
 * OUI       NON
 *  |         \
 *  |          \
 *  v           v
 * Copier le    Retourner NULL
 * mot dans la
 * nouvelle chaîne
 *     |
 *     v
 *   Retourner le pointeur vers la nouvelle chaîne
 *   Fin
 */
char	*alloc_copy_word_excluding_quot_with_tracking(\
char *word, t_quote *state, t_data *data)
{
	char	*str;
	int		i;
	int		j;

	i = ZERO_INIT;
	j = ZERO_INIT;
	while (word[i] && word[i] != ' ')/*         ---> condition non intelligible --> fonction         */
	{
		if ((word[i] == '"' && is_single_quote_closed(state))
			|| (word[i] == '\'' && is_doubl_quote_closed(state)))/*     ---> condition trop longue --> fonction     */
			i++;
		else
		{
			i++;
			j++;
		}
	}
	str = ft_malloc_with_tracking(data, sizeof(char) * (j + sizeof('\0')));
	if (!str)
		return (NULL);
	return (copy_alloc_memory_excluding_inactive_quot(word, state, str));
}
