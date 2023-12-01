
#include "../../include/minishell.h"

/**
 * @fonction: malloc_for_expand
 * @brève_description: Alloue de la mémoire pour une chaîne de caractères résultant de l'expansion d'une commande.
 * 
 * @param exp: Pointeur double vers un nœud de lexème contenant le mot à étendre.
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * @param env: Tableau de chaînes de caractères représentant les variables d'environnement.
 * 
 * @description_détaillée:
 *   La fonction 'malloc_for_expand' calcule d'abord la longueur nécessaire pour la chaîne résultant de l'expansion
 *   du mot contenu dans le nœud de lexème pointé par 'exp'. Elle utilise la fonction 'expansion_length_for_word'
 *   pour déterminer cette longueur. Ensuite, elle alloue la mémoire nécessaire pour contenir la chaîne expandidée.
 *   
 *   Pourquoi cette allocation est-elle nécessaire ?
 *   - Allocation dynamique : Permet de créer une chaîne de la taille exacte nécessaire pour accueillir le résultat 
 *     de l'expansion, ce qui est crucial pour éviter les dépassements ou les gaspillages de mémoire.
 *   - Préparation à l'expansion : Fournit un espace mémoire où la chaîne expandidée peut être construite.
 *
 * @valeur_de_retour: 
 *   Retourne un pointeur sur la chaîne allouée, ou NULL en cas d'échec d'allocation.
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si l'allocation de mémoire échoue, la fonction retourne NULL.
 *   - La fonction dépend de l'état de 'st' et du contenu de 'exp' pour calculer la taille d'allocation nécessaire.
 *
 * @exemples_d'utilisation:
 *   t_lexer *exp; // Initialisé et pointe vers un nœud spécifique
 *   t_quote st;
 *   char **env; // Tableau de variables d'environnement
 *   char *str = malloc_for_expand(&exp, &st, env); // Alloque de la mémoire pour l'expansion
 *
 * @dépendances:
 *   - expansion_length_for_word : Fonction pour calculer la longueur nécessaire de l'expansion.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser la longueur 'len' à ZERO_INIT et l'état de 'st->found' à ZERO_INIT
 *   |
 *   v
 * Vérifier si le premier caractère du mot est '$' et si c'est le seul caractère
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Incrémenter  Ajouter la longueur nécessaire pour l'expansion du mot
 * len         en utilisant 'expansion_length_for_word'
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
char	*malloc_for_expand(t_lexer **exp, t_quote *st, char **env)
{
	int		len;
	char	*str;

	len = ZERO_INIT;
	st->found = ZERO_INIT;
	if ((*exp)->word[0] && (*exp)->word[0] == '$' && !(*exp)->word[1])
		len++;
	expansion_length_for_word(exp, st, env, &len);
	str = ft_malloc_with_tracking(sizeof(char) * (len + sizeof('\0')));
	if (!str)
		return (NULL);
	return (str);
}

/**
 * @fonction: manage_expantion
 * @brève_description: Gère l'expansion et le remplacement des mots dans un nœud de lexème.
 * 
 * @param expnd: Pointeur double vers le nœud de lexème à traiter pour l'expansion.
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * @param nv: Tableau de chaînes de caractères contenant les nouveaux mots à insérer.
 * @param exp: Pointeur sur une structure t_expand contenant les détails de l'expansion.
 * 
 * @description_détaillée:
 *   La fonction 'manage_expantion' réalise l'expansion des mots dans un nœud de lexème donné. Elle commence 
 *   par calculer la longueur requise pour l'expansion et alloue la mémoire nécessaire. Si l'expansion est 
 *   nécessaire et qu'il n'y a pas de guillemets, elle divise la chaîne expandidée en plusieurs mots. Sinon, 
 *   elle supprime simplement les guillemets de la chaîne expandidée. Ensuite, elle remplace l'ancien nœud 
 *   par les nouveaux mots obtenus après l'expansion.
 *   
 *   Pourquoi gérer l'expansion de cette manière ?
 *   - Précision de l'interprétation : L'expansion correcte des mots est cruciale pour interpréter 
 *     correctement les commandes et les arguments, en particulier dans les environnements de shell.
 *   - Flexibilité dans le traitement des lexèmes : Permet de traiter de manière flexible les mots avec 
 *     ou sans guillemets et de gérer les cas où l'expansion produit plusieurs mots.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si l'allocation de mémoire pour 'exp->str' échoue, la fonction s'arrête sans effectuer d'expansion.
 *   - La modification de 'expnd' peut changer la structure de la liste de lexèmes.
 *
 * @exemples_d'utilisation:
 *   t_lexer *expnd; // Initialisé et pointe vers un nœud spécifique
 *   t_quote st;
 *   char *nv[] = { "nouveau_mot", NULL };
 *   t_expand exp;
 *   manage_expantion(&expnd, &st, nv, &exp); // Gère l'expansion pour le nœud expnd
 *
 * @dépendances:
 *   - Fonctions telles que malloc_for_expand, sum_expansion_length, split_word, remove_quote, etc.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser la longueur d'expansion dans 'exp->len' à ZERO_INIT
 *   |
 *   v
 * Convertir l'erreur de 'g_all.utils->err' en chaîne de caractères
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
 * Calculer la longueur totale de l'expansion avec 'sum_expansion_length'
 *   |
 *   v
 * Terminer la chaîne 'exp->str' avec le caractère nul '\0'
 *   |
 *   v
 * L'expansion est-elle nécessaire sans guillemets ('exp->need_expand' et 'exp->quote') ?
 *  /       \
 * OUI      NON
 *  |         |
 *  |         |
 *  v         v
 * Diviser   Supprimer les guillemets de 'exp->str'
 * 'exp->str'  et assigner le résultat à '(*expnd)->word'
 * en mots    avec 'remove_quote'
 * avec       puis retourner de la fonction
 * 'split_word'
 *   |
 *   v
 * Remplacer l'ancien nœud 'expnd' par les nouveaux mots avec 'replace_old_node'
 *   |
 *   v
 * Fin
 */
void	manage_expantion(t_lexer **expnd, t_quote *st, char **nv, t_expand *exp)
{	
	char	**expanded;

	exp->len = ZERO_INIT;
	g_all.utils->error = ft_itoa(g_all.utils->err);
	exp->str = malloc_for_expand(expnd, st, nv);
	if (!exp->str)
		return ;
	st->is_quote = ZERO_INIT;
	st->is_dquote = ZERO_INIT;
	sum_expansion_length(expnd, st, nv, exp);
	exp->str[exp->len] = '\0';
	if (exp->need_expand == 1 && exp->quote == 0)
		expanded = split_word(exp->str, st);
	else
	{
		(*expnd)->word = remove_quote(exp->str, st);
		return ;
	}
	replace_old_node(expnd, expanded);
}

/**
 * @fonction: add_back_new_node
 * @brève_description: Ajoute de nouveaux nœuds à une liste de lexèmes après un nœud spécifié.
 * 
 * @param insert: Tableau de chaînes de caractères à insérer dans les nouveaux nœuds.
 * @param back: Pointeur vers le nœud après lequel les nouveaux nœuds doivent être insérés.
 * @param next: Pointeur vers le nœud avant lequel les nouveaux nœuds doivent être insérés.
 * @param len: Longueur du tableau 'insert', indiquant le nombre de nœuds à ajouter.
 * 
 * @description_détaillée:
 *   La fonction 'add_back_new_node' crée et insère de nouveaux nœuds dans une liste de lexèmes. Pour chaque 
 *   élément de 'insert' (à partir du deuxième élément), elle crée un nouveau nœud, l'initialise avec le mot 
 *   correspondant, et l'insère dans la liste entre les nœuds 'back' et 'next'. Elle met à jour les liens 
 *   précédent et suivant pour maintenir l'intégrité de la liste.
 *   
 *   Pourquoi ajouter de nouveaux nœuds ?
 *   - Flexibilité de la structure de données : Permet d'insérer dynamiquement de nouveaux éléments dans 
 *     une liste de lexèmes, ce qui est crucial pour des opérations comme l'expansion de variables ou 
 *     l'ajout de commandes supplémentaires.
 *   - Conservation de l'ordre des lexèmes : Assure que les nouveaux nœuds sont insérés au bon endroit 
 *     pour préserver l'ordre logique des lexèmes.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si l'allocation de mémoire pour un nouveau nœud échoue, la fonction s'arrête sans ajouter de nœuds supplémentaires.
 *   - La fonction modifie la liste de lexèmes, ce qui peut affecter les itérations ou les références ultérieures à cette liste.
 *
 * @exemples_d'utilisation:
 *   char *insert[] = {"mot1", "mot2", "mot3", NULL};
 *   t_lexer *back =  nœud existant ;
 *   t_lexer *next =  nœud suivant ;
 *   add_back_new_node(insert, back, next, 3); // Ajoute "mot2" et "mot3" à la liste de lexèmes
 *
 * @dépendances:
 *   - ft_malloc_with_tracking : Fonction pour allouer de la mémoire avec suivi.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Initialiser l'index 'i' à ZERO_INIT
 *   |
 *   v
 * Entrer dans une boucle pour parcourir chaque élément de 'insert' (à partir du deuxième élément)
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
 * Configurer le nouveau nœud (tmp) avec les valeurs appropriées (mot, liens, token)
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
 * Retourner à la condition de la boucle pour traiter l'élément suivant de 'insert'
 *   |
 *   v
 * Fin de la boucle
 *   |
 *   v
 * Fin
 */
void	add_back_new_node(char **insert, t_lexer *back, t_lexer *next, int len)
{
	int		i;
	t_lexer	*tmp;

	i = ZERO_INIT;
	while (++i < len)
	{
		tmp = ft_malloc_with_tracking(sizeof(t_lexer) * (1));
		if (!tmp)
			return ;
		if (back)
			back->next = tmp;
		if (next)
			next->prev = tmp;
		tmp->prev = back;
		tmp->next = next;
		tmp->token = ARG;
		tmp->word = ft_strdup(insert[i]);
		back = tmp;
	}
}

/**
 * @fonction: replace_old_node
 * @brève_description: Remplace un nœud existant dans une liste de lexèmes par un ou plusieurs nouveaux nœuds.
 * 
 * @param old_node: Pointeur double vers un nœud existant dans la liste de lexèmes.
 * @param to_insert: Tableau de chaînes de caractères à insérer à la place du nœud existant.
 * 
 * @description_détaillée:
 *   La fonction 'replace_old_node' remplace le contenu du nœud pointé par 'old_node' par le premier élément 
 *   du tableau 'to_insert'. Si 'to_insert' contient plusieurs éléments, elle insère les éléments supplémentaires 
 *   dans de nouveaux nœuds après 'old_node'. Elle gère les connexions de la liste de lexèmes pour maintenir 
 *   l'intégrité de la liste après l'insertion.
 *   
 *   Pourquoi cette opération est-elle nécessaire ?
 *   - Modification dynamique des lexèmes : Permet de changer le contenu d'un lexème ou d'ajouter de nouveaux 
 *     lexèmes en fonction des besoins, ce qui est utile pour des opérations comme l'expansion de variables 
 *     ou le remplacement de commandes.
 *   - Maintien de la structure de la liste : Assure que la liste de lexèmes reste cohérente et correctement 
 *     liée après la modification.
 *
 * @valeur_de_retour: Aucune (void).
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'old_node' est NULL ou ne pointe pas vers un nœud valide, le comportement est indéfini.
 *   - La modification de la liste de lexèmes peut affecter l'ensemble de la structure des données.
 *
 * @exemples_d'utilisation:
 *   t_lexer *old_node; // Initialisé et pointe vers un nœud spécifique
 *   char *to_insert[] = {"nouveau_mot", NULL};
 *   replace_old_node(&old_node, to_insert); // Remplace le contenu de old_node par "nouveau_mot"
 *
 * @dépendances:
 *   - len_darr : Fonction pour calculer la longueur du tableau 'to_insert'.
 *   - add_back_new_node : Fonction pour ajouter de nouveaux nœuds dans la liste de lexèmes.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Calculer la longueur de 'to_insert' avec len_darr
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
void	replace_old_node(t_lexer **old_node, char **to_insert)
{
	int		len;
	t_lexer	*back;
	t_lexer	*next;

	len = len_darr(to_insert);
	back = (*old_node)->prev;
	next = (*old_node)->next;
	(*old_node)->word = ft_strdup(to_insert[0]);
	if (len > 1)
	{
		back = (*old_node);
		add_back_new_node(to_insert, back, next, len);
	}
}

/**
 * @fonction: put_word_without_quote
 * @brève_description: Copie une chaîne en omettant les guillemets, en tenant compte de l'état des guillemets.
 * 
 * @param old: Chaîne de caractères source contenant éventuellement des guillemets.
 * @param nw: Chaîne de caractères cible où la chaîne modifiée sera stockée.
 * @param state: Pointeur sur une structure t_quote représentant l'état actuel des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'put_word_without_quote' parcourt la chaîne 'old', copiant chaque caractère dans 'nw' 
 *   sauf les guillemets, tout en mettant à jour et en respectant l'état des guillemets (simples ou doubles). 
 *   Elle gère correctement les guillemets imbriqués et assure que seuls les guillemets non imbriqués sont 
 *   omis.
 *   
 *   Pourquoi cette fonction est-elle utile ?
 *   - Nettoyage des chaînes : Permet de retirer les guillemets des chaînes de caractères, ce qui est souvent 
 *     nécessaire après l'analyse des commandes ou des arguments.
 *   - Prise en compte de l'état des guillemets : Assure que les guillemets imbriqués sont traités correctement, 
 *     ce qui est essentiel pour une interprétation précise des chaînes.
 *
 * @valeur_de_retour: 
 *   Retourne un pointeur sur la chaîne 'nw' après la copie et la modification.
 *
 * @erreurs_possibles_et_effets_de_bord:
 *   - Si 'old' ou 'nw' sont NULL, le comportement de la fonction est indéfini.
 *   - La fonction modifie 'state' en fonction des guillemets rencontrés dans 'old'.
 *
 * @exemples_d'utilisation:
 *   char old[] = "hello \"world\"";
 *   char nw[50];
 *   t_quote state;
 *   char *result = put_word_without_quote(old, nw, &state); // Résultat attendu : "hello world"
 *
 * @dépendances:
 *   - update_quoting_state : Fonction pour mettre à jour l'état des guillemets.
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
char	*put_word_without_quote(char *old, char *nw, t_quote *state)
{
	int	i;
	int	j;

	i = -1;
	j = ZERO_INIT;
	state->is_quote = ZERO_INIT;
	state->is_dquote = ZERO_INIT;
	while (old[++i])
	{
		if (old[i] == '"' && state->is_quote == 0)
		{
			update_quoting_state(old[i], state);
			continue ;
		}
		if (old[i] == '\'' && state->is_dquote == 0)
		{
			update_quoting_state(old[i], state);
			continue ;
		}
		nw[j++] = old[i];
	}
	nw[j] = '\0';
	return (nw);
}
