#include "../../include/minishell.h"

/**
 * @fonction: allocate_memory_for_expanded_word
 * @breve_description: Alloue de la mémoire pour une chaîne de caractères
 * résultant de l'expansion d'une commande.
 * 
 * @param exp: Pointeur double vers un nœud de lexème contenant le mot à
 * étendre.
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel
 * des guillemets.
 * @param env: Tableau de chaînes de caractères représentant les variables
 * d'environnement.
 * 
 * @description_detaillee:
 * La fonction 'allocate_memory_for_expanded_word' calcule d'abord la
 * longueur nécessaire pour la chaîne résultant de l'expansion
 * du mot contenu dans le nœud de lexème pointé par 'exp'. Elle utilise la
 * fonction 'length_of_env_variable_value' pour déterminer cette longueur.
 * Ensuite, elle alloue la mémoire nécessaire pour contenir la chaîne
 * expandidée.
 *   
 * Pourquoi cette allocation est-elle nécessaire ?
 * - Allocation dynamique : Permet de créer une chaîne de la taille exacte
 * nécessaire pour accueillir le résultat de l'expansion, ce qui est crucial
 * pour éviter les dépassements ou les gaspillages de mémoire.
 * - Préparation à l'expansion : Fournit un espace mémoire où la chaîne
 * expandidée peut être construite.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur sur la chaîne allouée, ou NULL en cas d'échec
 * d'allocation.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si l'allocation de mémoire échoue, la fonction retourne NULL.
 * - La fonction dépend de l'état de 'st' et du contenu de 'exp' pour calculer
 * la taille d'allocation nécessaire.
 *
 * @exemples_utilisation:
 * t_lexer *exp; // Initialisé et pointe vers un nœud spécifique
 * t_quote st;
 * char **env; // Tableau de variables d'environnement
 * char *str = allocate_memory_for_expanded_word(&exp, &st, env);
 * ------------> Alloque de la mémoire pour l'expansion
 *
 * @dependances:
 * - length_of_env_variable_value : Fonction pour calculer la longueur
 * nécessaire de l'expansion.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser
 * la longueur 'len' à ZERO_INIT
 *            et
 * l'état de 'st->found' à ZERO_INIT
 *            |
 *            v
 *       Vérifier si
 * le premier caractère du mot
 *         est '$'
 *           et
 * si c'est le seul caractère
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Incrémenter  Ajouter la longueur nécessaire pour l'expansion du mot
 * len         en utilisant 'length_of_env_variable_value'
 *   |
 *   v
 * Allouer de la mémoire pour la chaîne 'str' de taille 'len + sizeof('\0')'
 *   |
 *   v
 * La mémoire a-t-elle été allouée avec succès pour 'str' ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Retourner  Retourner NULL (échec de l'allocation)
 * le pointeur
 * sur 'str'
 * Fin
 */
char	*allocate_memory_for_expanded_word(\
t_lexer **exp, t_quote *st, char **env, t_data *data)
{
	int		len;
	char	*str;

	env = NULL;
	len = ZERO_INIT;
	st->var_env_match_confirmed = ZERO_INIT;
	if (is_dollar_char_then_end_of_string(*exp))
		len++;
	length_of_env_variable_value(exp, st, data, &len);
	str = ft_malloc_with_tracking(data, sizeof(char) * (len + sizeof('\0')));
	if (!str)
		return (NULL);
	return (str);
}

/**
 * @fonction: expand_and_insert_in_lexeme_linked_list
 * @breve_description: Gère l'expansion et le remplacement des mots dans un
 * nœud de lexème.
 * 
 * @param expnd: Pointeur double vers le nœud de lexème à traiter pour
 * l'expansion.
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel
 * des guillemets.
 * @param nv: Tableau de chaînes de caractères contenant les nouveaux mots à
 * insérer.
 * @param exp: Pointeur sur une structure t_expand contenant les détails de
 * l'expansion.
 * 
 * @description_detaillee:
 * La fonction 'expand_and_insert_in_lexeme_linked_list' réalise l'expansion
 * des mots dans un nœud de lexème donné. Elle commence par calculer la
 * longueur requise pour l'expansion et alloue la mémoire nécessaire. Si
 * l'expansion est nécessaire et qu'il n'y a pas de guillemets, elle divise la
 * chaîne expandidée en plusieurs mots. Sinon, elle supprime simplement les
 * guillemets de la chaîne expandidée. Ensuite, elle remplace l'ancien nœud par
 * les nouveaux mots obtenus après l'expansion.
 *   
 * Pourquoi gérer l'expansion de cette manière ?
 * - Précision de l'interprétation : L'expansion correcte des mots est cruciale
 * pour interpréter correctement les commandes et les arguments, en particulier
 * dans les environnements de shell.
 * - Flexibilité dans le traitement des lexèmes : Permet de traiter de manière
 * flexible les mots avec ou sans guillemets et de gérer les cas où l'expansion
 * produit plusieurs mots.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si l'allocation de mémoire pour 'exp->str' échoue, la fonction s'arrête
 * sans effectuer d'expansion.
 * - La modification de 'expnd' peut changer la structure de la liste de
 * lexèmes.
 *
 * @exemples_utilisation:
 * t_lexer *expnd; // Initialisé et pointe vers un nœud spécifique
 * t_quote st;
 * char *nv[] = { "nouveau_mot", NULL };
 * t_expand exp;
 * expand_and_insert_in_lexeme_linked_list(&expnd, &st, nv, &exp);
 * -------> Gère l'expansion pour le nœud expnd
 *
 * @dependances:
 * - Fonctions telles que allocate_memory_for_expanded_word,
 * expand_variables_and_handle_special_chars, split_word_by_quotes,
 * create_cleaned_str_excluding_inactive_quots, etc.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser la longueur d'expansion dans 'exp->len' à ZERO_INIT
 *   |
 *   v
 * Convertir l'erreur de 'globi' en chaîne de caractères
 *   |
 *   v
 * Allouer de la mémoire pour la chaîne d'expansion 'exp->str'
 *   |
 *   v
 * La mémoire a-t-elle été allouée avec succès pour 'exp->str' ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Continuer  Sortir de la fonction (arrêter l'exécution)
 * l'exécution
 *   |
 *   v
 * Réinitialiser les états de guillemets dans 'st'
 *   |
 *   v
 * Calculer la longueur
 * totale de 
 * l'expansion 
 * avec 'expand_variables_and_handle_special_chars'
 *   |
 *   v
 * Terminer la chaîne 'exp->str' avec le caractère nul '\0'
 *   |
 *   v
 * L'expansion est-elle nécessaire sans guillemets
 * ('exp->need_expand' et 'exp->quote') ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Diviser   Supprimer les guillemets de 'exp->str'
 * 'exp->str'  et assigner le résultat à '(*expnd)->word'
 * en mots    avec 'create_cleaned_str_excluding_inactive_quots'
 * avec       puis retourner de la fonction
 * 'split_word_by_quotes'
 *   |
 *   v
 * Remplacer l'ancien nœud 'expnd'
 * par les nouveaux mots avec 
 * 'replace_and_extend_chained_list_lexeme'
 *   |
 *   v
 * Fin
 */
void	expand_and_insert_in_lexeme_linked_list(\
t_lexer **expnd, t_quote *st, t_data *data, t_expand *exp)
{
	char	**expanded;

	exp->length_of_expanded_var_value = ZERO_INIT;
	data->utils->g_signal_in_char_format = \
	convert_int_to_string_with_tracking(data, g_signal_received);
	exp->value_of_expanded_var_from_env = \
	allocate_memory_for_expanded_word(expnd, st, data->full_env_var_copy_gamma, data);
	if (!exp->value_of_expanded_var_from_env)/*         ---> condition non intelligible --> fonction         */
		return ;
	st->singl_quot_status = ZERO_INIT;
	st->doubl_quot_status = ZERO_INIT;
	expand_variables_and_handle_special_chars(expnd, st, exp, data);
	exp->value_of_expanded_var_from_env[exp->length_of_expanded_var_value] = '\0';
	if (is_expansion_required_and_unquoted(exp))
		expanded = split_word_by_quotes(data, exp->value_of_expanded_var_from_env, st);
	else
	{
		(*expnd)->word = \
		create_cleaned_str_excluding_inactive_quots(exp->value_of_expanded_var_from_env, st, data);
		return ;
	}
	replace_and_extend_chained_list_lexeme(expnd, expanded, data);
}

/**
 * @fonction: insert_additional_lexemes
 * @breve_description: Ajoute de nouveaux nœuds à une liste de lexèmes après
 * un nœud spécifié.
 * 
 * @param insert: Tableau de chaînes de caractères à insérer dans les nouveaux
 * nœuds.
 * @param back: Pointeur vers le nœud après lequel les nouveaux nœuds doivent
 * être insérés.
 * @param next: Pointeur vers le nœud avant lequel les nouveaux nœuds doivent
 * être insérés.
 * @param len: Longueur du tableau 'insert', indiquant le nombre de nœuds à
 * ajouter.
 * 
 * @description_detaillee:
 * La fonction 'insert_additional_lexemes' crée et insère de nouveaux nœuds
 * dans une liste de lexèmes. Pour chaque élément de 'insert' (à partir du
 * deuxième élément), elle crée un nouveau nœud, l'initialise avec le mot 
 * correspondant, et l'insère dans la liste entre les nœuds 'back' et 'next'.
 * Elle met à jour les liens précédent et suivant pour maintenir l'intégrité
 * de la liste.
 *   
 * Pourquoi ajouter de nouveaux nœuds ?
 * - Flexibilité de la structure de données : Permet d'insérer dynamiquement de
 * nouveaux éléments dans une liste de lexèmes, ce qui est crucial pour des
 * opérations comme l'expansion de variables ou l'ajout de commandes
 * supplémentaires.
 * - Conservation de l'ordre des lexèmes : Assure que les nouveaux nœuds sont
 * insérés au bon endroit pour préserver l'ordre logique des lexèmes.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si l'allocation de mémoire pour un nouveau nœud échoue, la fonction
 * s'arrête sans ajouter de nœuds supplémentaires.
 * - La fonction modifie la liste de lexèmes, ce qui peut affecter les
 * itérations ou les références ultérieures à cette liste.
 *
 * @exemples_utilisation:
 * char *insert[] = {"mot1", "mot2", "mot3", NULL};
 * t_lexer *back =  nœud existant ;
 * t_lexer *next =  nœud suivant ;
 * insert_additional_lexemes(insert, back, next, 3);
 * -------> Ajoute "mot2" et "mot3" à la liste de lexèmes
 *
 * @dependances:
 * - ft_malloc_with_tracking : Fonction pour allouer de la mémoire avec suivi.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser l'index 'i' à ZERO_INIT
 *   |
 *   v
 * Entrer dans une boucle
 * pour parcourir chaque élément de 'insert'
 * (à partir du deuxième élément)
 *   |
 *   v
 * Allouer de la mémoire pour un nouveau nœud (tmp)
 *   |c
 *   v
 * La mémoire a-t-elle été allouée avec succès ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Continuer  Sortir de la boucle et arrêter l'exécution
 * l'exécution
 *   |
 *   v
 * Configurer le nouveau nœud (tmp) avec les valeurs appropriées
 * (mot, liens, token)
 *   |
 *   v
 * Insérer le nouveau nœud (tmp) dans la liste entre 'back' et 'next'
 *   |
 *   v
 * Mettre à jour 'back' pour pointer vers le nouveau nœud (tmp)
 *   |
 *   v
 * Incrémenter l'index 'i'
 *   |
 *   v
 * Retourner 
 * à la condition de la boucle
 * pour traiter l'élément suivant de 'insert'
 *   |
 *   v
 * Fin de la boucle
 *   |
 *   v
 * Fin
 */
void	insert_additional_lexemes(\
char **insert, t_lexer *back, t_data *data, int len)
{
	int		i;
	t_lexer	*tmp;

	i = ZERO_INIT;
	while (is_there_remaining_elements(++i, len))
	{
		tmp = ft_malloc_with_tracking(data, sizeof(t_lexer) * (1));
		if (!tmp)
			return ;
		if (back)
			back->next = tmp;
		if (data->lexer_list->next)/*         ---> condition non intelligible --> fonction         */
			data->lexer_list->next->prev = tmp;
		tmp->prev = back;
		tmp->next = data->lexer_list->next;
		tmp->token = ARG;
		tmp->word = ft_strdup(data, insert[i]);
		back = tmp;
	}
}

/**
 * @fonction: replace_and_extend_chained_list_lexeme
 * @breve_description: Remplace un nœud existant dans une liste de lexèmes par
 * un ou plusieurs nouveaux nœuds.
 * 
 * @param old_node: Pointeur double vers un nœud existant dans la liste de
 * lexèmes.
 * @param to_insert: Tableau de chaînes de caractères à insérer à la place du
 * nœud existant.
 * 
 * @description_detaillee:
 * La fonction 'replace_and_extend_chained_list_lexeme' remplace le contenu du
 * nœud pointé par 'old_node' par le premier élément du tableau 'to_insert'.
 * Si 'to_insert' contient plusieurs éléments, elle insère les éléments
 * supplémentaires dans de nouveaux nœuds après 'old_node'.
 * Elle gère les connexions de la liste de lexèmes pour maintenir l'intégrité
 * de la liste après l'insertion.
 *   
 * Pourquoi cette opération est-elle nécessaire ?
 * - Modification dynamique des lexèmes : Permet de changer le contenu d'un
 * lexème ou d'ajouter de nouveaux lexèmes en fonction des besoins, ce qui est
 * utile pour des opérations comme l'expansion de variables ou le remplacement
 * de commandes.
 * - Maintien de la structure de la liste : Assure que la liste de lexèmes
 * reste cohérente et correctement liée après la modification.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'old_node' est NULL ou ne pointe pas vers un nœud valide, le
 * comportement est indéfini.
 * - La modification de la liste de lexèmes peut affecter l'ensemble de la
 * structure des données.
 *
 * @exemples_utilisation:
 * t_lexer *old_node; // Initialisé et pointe vers un nœud spécifique
 * char *to_insert[] = {"nouveau_mot", NULL};
 * replace_and_extend_chained_list_lexeme(&old_node, to_insert);
 * -------> Remplace le contenu de old_node par "nouveau_mot"
 *
 * @dependances:
 * - count_string_array_elements : Fonction pour calculer la longueur du
 * tableau 'to_insert'.
 * - insert_additional_lexemes : Fonction pour ajouter de nouveaux nœuds dans
 * la liste de lexèmes.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Calculer la longueur de 'to_insert' avec count_string_array_elements
 *     |
 *     v
 *   Remplacer le mot dans 'old_node' par le premier élément de 'to_insert'
 *     |
 *     v
 *   'to_insert' contient-il plus d'un élément ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Ajouter   Fin
 * des nœuds supplémentaires
 * pour les éléments restants
 */
void	replace_and_extend_chained_list_lexeme(\
t_lexer **old_node, char **to_insert, t_data *data)
{
	int		len;
	t_lexer	*back;

	len = count_string_array_elements(to_insert);
	back = (*old_node)->prev;
	(*old_node)->word = ft_strdup(data, to_insert[0]);
	if (len > 1)
	{
		back = (*old_node);
		insert_additional_lexemes(to_insert, back, data, len);
	}
}

/**
 * @fonction: copy_string_excluding_quotes
 * @breve_description: Copie une chaîne en omettant les guillemets, en tenant
 * compte de l'état des guillemets.
 * 
 * @param old: Chaîne de caractères source contenant éventuellement des
 * guillemets.
 * @param nw: Chaîne de caractères cible où la chaîne modifiée sera stockée.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel
 * des guillemets.
 * 
 * @description_detaillee:
 * La fonction 'copy_string_excluding_quotes' parcourt la chaîne 'old',
 * copiant chaque caractère dans 'nw' sauf les guillemets, tout en mettant à
 * jour et en respectant l'état des guillemets (simples ou doubles). Elle gère
 * correctement les guillemets imbriqués et assure que seuls les guillemets
 * non imbriqués sont omis.
 *   
 * Pourquoi cette fonction est-elle utile ?
 * - Nettoyage des chaînes : Permet de retirer les guillemets des chaînes de
 * caractères, ce qui est souvent nécessaire après l'analyse des commandes ou
 * des arguments.
 * - Prise en compte de l'état des guillemets : Assure que les guillemets
 * imbriqués sont traités correctement, ce qui est essentiel pour une
 * interprétation précise des chaînes.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur sur la chaîne 'nw' après la copie et la modification.
 *
 * @erreurs_possibles_et_effets_de_bord:
 * - Si 'old' ou 'nw' sont NULL, le comportement de la fonction est indéfini.
 * - La fonction modifie 'state' en fonction des guillemets rencontrés dans
 * 'old'.
 *
 * @exemples_utilisation:
 * char old[] = "hello \"world\"";
 * char nw[50];
 * t_quote state;
 * char *result = copy_string_excluding_quotes(old, nw, &state);
 * -------> Résultat attendu : "hello world"
 *
 * @dependances:
 * - update_quoting_state : Fonction pour mettre à jour l'état des guillemets.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser les indices 'i' et 'j' et l'état des guillemets
 *     |
 *     v
 *   Parcourir la chaîne 'old'
 *     |
 *     v
 *   Le caractère actuel est-il un guillemet à ignorer ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Mettre à  Copier le caractère de 'old' à 'nw'
 * jour l'état  et incrémenter 'j'
 * des guillemets
 *  |
 *  v
 * Continuer jusqu'à la fin de la chaîne 'old'
 *  |
 *  v
 * Ajouter le caractère de fin de chaîne ('\0') à 'nw'
 *  |
 *  v
 * Retourner 'nw'
 */
char	*copy_string_excluding_quotes(\
char *original_cmd_with_quotes, \
char *cmd_without_quotes_ready_for_expand, t_quote *state)
{
	int	i;
	int	j;

	i = -1;
	j = ZERO_INIT;
	state->singl_quot_status = ZERO_INIT;
	state->doubl_quot_status = ZERO_INIT;
	while (original_cmd_with_quotes[++i])
	{
		if (is_double_quote_with_closed_single_quotes(\
		original_cmd_with_quotes[i], state))
		{
			update_quoting_state(original_cmd_with_quotes[i], state);
			continue ;
		}
		if (is_single_quote_with_closed_double_quotes(\
		original_cmd_with_quotes[i], state))
		{
			update_quoting_state(original_cmd_with_quotes[i], state);
			continue ;
		}
		cmd_without_quotes_ready_for_expand[j++] = original_cmd_with_quotes[i];
	}
	cmd_without_quotes_ready_for_expand[j] = '\0';
	return (cmd_without_quotes_ready_for_expand);
}
