
#include "../../include/minishell.h"

/**
 * @nom: single_quote_expantion
 * @brève_description: Effectue une expansion de chaîne de caractères en tenant compte des guillemets simples.
 * 
 * @param word: char *word, pointeur vers la chaîne de caractères à traiter.
 * @param exp: t_expand *exp, pointeur vers une structure gérant l'expansion.
 * 
 * @description_détaillée:
 *   La fonction 'single_quote_expantion' parcourt la chaîne 'word' et ajoute chaque caractère à la chaîne 
 *   de la structure 'exp', jusqu'à rencontrer un guillemet simple ou la fin de la chaîne. Cette fonction est 
 *   utilisée pour gérer l'expansion des chaînes de caractères encadrées par des guillemets simples, où l'interprétation 
 *   des caractères est généralement littérale.
 *   
 *   Pourquoi effectuer une expansion avec des guillemets simples ?
 *   - Interprétation Littérale : Dans de nombreux langages de script et shells, les guillemets simples sont utilisés 
 *     pour indiquer que le contenu doit être traité de manière littérale, sans interprétation des caractères spéciaux 
 *     ou des variables.
 *   - Gestion des Chaînes de Caractères : Permet de traiter correctement les chaînes encapsulées dans des guillemets 
 *     simples, en conservant leur forme originale.
 *
 * @valeur_de_retour: 
 *   Retourne l'indice jusqu'où l'expansion a été effectuée dans la chaîne 'word'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'word' est NULL, le comportement est indéfini.
 *   - Modifie 'exp' en ajoutant les caractères à 'exp->str' et en mettant à jour 'exp->len'.
 *
 * @exemples_d'utilisation:
 *   char *word = "'Ceci est un test'";
 *   t_expand exp;
 *   init_expand_structure(&exp);
 *   int indice = single_quote_expantion(word, &exp);
 *
 * @dépendances: Aucune.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser la variable 'index' à -1
 *   |
 *   v
 *   Entrer dans une boucle pour parcourir 'word'
 *     |
 *     v
 *   - Vérifier si le caractère courant est un guillemet simple ou la fin de 'word'
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Ajouter le caractère courant à 'exp->str'  - Retourner 'index'
 *   - Incrémenter 'exp->len' et 'index'
 *   |
 *   \----------------------------------/
 *   |
 *   v
 *   Fin
 */
int	single_quote_expantion(char *word, t_expand *exp)
{
	int	index;

	index = -1;
	while (word[++index] && word[index] != '\'')
	{
		exp->str[exp->len++] = word[index];
	}
	return (index);
}

/**
 * @nom: word_not_found
 * @brève_description: Traite le cas où un mot d'expansion n'est pas trouvé.
 * 
 * @param w: char *w, pointeur vers la chaîne de caractères contenant le mot à traiter.
 * @param state: t_quote *state, pointeur vers une structure représentant l'état des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'word_not_found' est appelée lorsqu'un mot d'expansion n'est pas trouvé dans une chaîne. 
 *   Elle parcourt la chaîne 'w' à partir de l'indice 1, en recherchant soit la fin de la chaîne, soit 
 *   le caractère '$', soit un caractère spécial traité par 'is_special_syntax_character'. La fonction 
 *   sert à déterminer jusqu'où aller dans la chaîne en cas d'échec d'expansion.
 *   
 *   Pourquoi gérer les cas de mots non trouvés ?
 *   - Éviter des Traitements Incorrects : Empêche l'expansion de parties de la chaîne qui ne correspondent 
 *     pas à des variables d'environnement valides, assurant une interprétation correcte.
 *   - Gestion des Caractères Spéciaux : Permet de traiter correctement les caractères ayant une signification 
 *     syntaxique spéciale, évitant ainsi des erreurs de traitement.
 *
 * @valeur_de_retour: 
 *   Retourne l'indice jusqu'où la recherche a été effectuée dans la chaîne 'w'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'w' est NULL, le comportement est indéfini.
 *   - La fonction ne modifie ni 'w' ni 'state'.
 *
 * @exemples_d'utilisation:
 *   char *word = "$varNonExistante";
 *   t_quote state = {0, 0};
 *   int indice = word_not_found(word, &state);
 *
 * @dépendances: 
 *   - is_special_syntax_character pour vérifier les caractères spéciaux.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser la variable 'i' à 1
 *   |
 *   v
 *   - Vérifier si 'w[i]' n'est pas nul
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Entrer dans une boucle    - Retourner 'i'
 *     pour parcourir 'w'
 *     |
 *     v
 *   - Vérifier si 'w[i]' est '$' ou la fin de la chaîne
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Vérifier si 'w[i]' est un  - Retourner 'i'
 *     caractère spécial
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Incrémenter 'i'            - Retourner 'i'
 *   |
 *   \----------------------------/
 *   |
 *   v
 *   Fin
 */
int	word_not_found(char *w, t_quote *state)
{
	int	i;

	i = 1;
	if (w[i])
	{
		while (w[i] != '\0' && w[i] != '$')
		{
			if (is_special_syntax_character(w[i], state))
				return (i);
			i++;
		}
	}
	return (i);
}

/**
 * @nom: put_error_code
 * @brève_description: Insère le code d'erreur courant dans la structure d'expansion.
 * 
 * @param exp: t_expand *exp, pointeur vers la structure d'expansion à mettre à jour.
 * 
 * @description_détaillée:
 *   La fonction 'put_error_code' ajoute le code d'erreur stocké dans 'data->utils->error' 
 *   à la chaîne de caractères dans la structure 'exp'. Elle parcourt le code d'erreur 
 *   caractère par caractère et les ajoute à 'exp->str', en mettant à jour 'exp->len' 
 *   pour chaque caractère ajouté. Cette fonction est utilisée principalement dans le 
 *   contexte de l'expansion de variables dans un shell, pour gérer les erreurs d'expansion.
 *   
 *   Pourquoi insérer un code d'erreur dans la chaîne d'expansion ?
 *   - Gestion des Erreurs : Permet de refléter les erreurs rencontrées pendant l'expansion 
 *     des variables dans le résultat final, ce qui est crucial pour le débogage et le 
 *     traitement des erreurs dans un shell ou un script.
 *   - Communication d'État : Fournit un moyen direct d'indiquer à l'utilisateur ou au 
 *     système qu'une erreur a eu lieu, en l'incorporant dans la chaîne de sortie.
 *
 * @valeur_de_retour: 
 *   Retourne un entier indiquant la position jusqu'où l'insertion a été effectuée.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction modifie 'exp', en ajoutant le code d'erreur à 'exp->str' et en mettant 
 *     à jour 'exp->len'.
 *   - Aucune vérification n'est faite pour s'assurer que 'exp->str' a suffisamment de place.
 *
 * @exemples_d'utilisation:
 *   t_expand exp;
 *   init_expand_structure(&exp);
 *   data->utils->error = "127";
 *   int position = put_error_code(&exp);
 *
 * @dépendances: 
 *   Aucune dépendance externe autre que la structure globale 'g_all'.
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
 *   /       \
 *  NON      OUI
 *   |         |
 *   v         v
 *   - Ajouter le caractère courant à 'exp->str'  - Marquer 'exp->found' comme 1 (trouvé)
 *   - Incrémenter 'exp->len' et 'i'              - Retourner la position actuelle (2)
 *   |
 *   \----------------------------------/
 *   |
 *   v
 *   Fin
 */
int	put_error_code(t_expand *exp, t_data *data)
{
	int	i;

	i = -1;
	while (data->utils->error[++i])
		exp->str[exp->len++] = data->utils->error[i];
	exp->found = 1;
	return (2);
}

/**
 * @nom: search_word
 * @brève_description: Recherche une correspondance entre une chaîne de caractères et une variable d'environnement.
 * 
 * @param w: char *w, pointeur vers la chaîne de caractères à rechercher.
 * @param exp: t_expand *exp, pointeur vers une structure gérant l'expansion.
 * @param nv: char *nv, pointeur vers la chaîne de caractères représentant une variable d'environnement.
 * @param state: t_quote *state, pointeur vers une structure représentant l'état des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'search_word' compare la chaîne 'w' avec la variable 'nv' pour trouver une correspondance. 
 *   Elle gère les cas spéciaux, comme le caractère '?', et vérifie si 'w' correspond à la partie nom de 'nv'. 
 *   Si une correspondance est trouvée et confirmée par des critères syntaxiques, elle ajoute la valeur de 
 *   la variable à la structure 'exp'.
 *   
 *   Pourquoi comparer les chaînes avec les variables d'environnement ?
 *   - Interprétation Correcte des Variables : Essentiel pour remplacer les noms de variables par leurs 
 *     valeurs dans des contextes comme les shells ou les scripts, garantissant ainsi une exécution précise.
 *   - Gestion des Cas Spéciaux : Traite les caractères spéciaux comme '?' de manière appropriée, 
 *     reflétant leur signification particulière dans certains contextes.
 *
 * @valeur_de_retour: 
 *   Retourne l'indice jusqu'où la recherche a été effectuée ou un code spécifique en cas de caractère '?'.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si 'w' ou 'nv' sont NULL, le comportement est indéfini.
 *   - Modifie 'exp' pour indiquer si une correspondance a été trouvée.
 *
 * @exemples_d'utilisation:
 *   char *word = "$HOME";
 *   t_expand exp;
 *   char *env_var = "HOME=/home/user";
 *   t_quote state = {0, 0};
 *   int indice = search_word(word, &exp, env_var, &state);
 *
 * @dépendances: 
 *   - put_error_code pour gérer les cas spéciaux.
 *   - is_special_syntax_character pour vérifier les caractères syntaxiques.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser les variables 'i' et 'y'
 *   |
 *   v
 *   - Vérifier si 'w[1]' est le caractère spécial '?'
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Appeler put_error_code   - Comparer les caractères de 'w' et 'nv'
 *   - Retourner le résultat    /       \
 *                              OUI      NON
 *                               |         |
 *                               v         v
 *   - Vérifier la fin de mot   - Continuer la comparaison
 *   - et correspondance '='    /       \
 *   /       \                 OUI      NON
 *  OUI      NON                |         |
 *   |         |                v         v
 *   v         v            - Ajouter la valeur    - Retourner l'indice 'i'
 *   - Mettre à jour 'exp'   - de 'nv' à 'exp->str'
 *   - Retourner 'i'         - Retourner 'i'
 *   |
 *   v
 *   Fin
 */
int	search_word(char *w, t_expand *exp, t_data *data, t_quote *state)
{
	int	i;
	int	y;

	i = -1;
	y = 0;
	if (w[1] == '?')
		return (put_error_code(exp, data));
	i = 0;
	while (w[i++] && data->nv[data->x][y] && w[i] == data->nv[data->x][y] && w[i] != '=')
		y++;
	if ((w[i] == '\0' || w[i] == '$' || is_special_syntax_character(w[i], state))
		&& data->nv[data->x][y] && data->nv[data->x][y] == '=')
	{
		exp->found = 1;
		while (data->nv[data->x][++y])
			exp->str[exp->len++] = data->nv[data->x][y];
		return (i);
	}
	return (i);
}

/**
 * @nom: basic_expantion
 * @brève_description: Effectue une expansion basique sur une chaîne de caractères.
 * 
 * @param w: char *w, pointeur vers la chaîne de caractères à étendre.
 * @param exp: t_expand *exp, pointeur vers une structure gérant l'expansion.
 * @param nv: char **nv, tableau de pointeurs vers des chaînes représentant des variables.
 * @param state: t_quote *state, pointeur vers une structure représentant l'état des guillemets.
 * 
 * @description_détaillée:
 *   La fonction 'basic_expantion' parcourt un tableau de chaînes (variables) pour trouver 
 *   une correspondance avec le début de la chaîne 'w'. Elle utilise 'search_word' pour comparer 
 *   chaque variable avec 'w'. Si une correspondance est trouvée ou si un caractère spécial '?' 
 *   est détecté, elle effectue l'expansion nécessaire. La fonction gère également les cas où 
 *   aucune correspondance n'est trouvée en appelant 'word_not_found'.
 *   
 *   Pourquoi effectuer une expansion basique ?
 *   - Interprétation des Variables : Permet de remplacer les noms de variables par leurs valeurs 
 *     correspondantes, ce qui est essentiel pour l'exécution correcte des commandes dans un shell.
 *   - Gestion des Caractères Spéciaux : Traite des caractères spéciaux comme '?', qui peuvent avoir 
 *     une signification particulière dans certains contextes, comme les opérations conditionnelles.
 *
 * @valeur_de_retour: 
 *   Retourne l'indice jusqu'où l'expansion a été effectuée ou un code d'erreur si aucune expansion n'est possible.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Si aucune correspondance n'est trouvée, appelle 'word_not_found'.
 *   - La fonction modifie 'exp' pour indiquer si une expansion a été trouvée.
 *
 * @exemples_d'utilisation:
 *   char *word = "$HOME";
 *   t_expand exp;
 *   char *variables[] = {"HOME=/home/user", NULL};
 *   t_quote state = {0, 0};
 *   int indice = basic_expantion(word, &exp, variables, &state);
 *
 * @dépendances: 
 *   - search_word pour rechercher des correspondances.
 *   - word_not_found pour gérer les cas sans correspondance.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 *   - Initialiser les variables 'i', 'x', et 'y'
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
 *   - Appeler search_word       - Comparer 'w[i]' avec 'nv[x][y]'
 *   - Retourner le résultat     /       \
 *                               OUI      NON
 *                                |         |
 *                                v         v
 *   - Appeler search_word       - Continuer avec la prochaine variable dans 'nv'
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
 *   /       \
 *  OUI      NON
 *   |         |
 *   v         v
 *   - Appeler word_not_found   - Continuer le processus
 *   - Retourner le résultat
 *   |
 *   v
 *   Fin
 */
int	basic_expantion(char *w, t_expand *exp, t_data *data, t_quote *state)
{
	int	i;
	int	y;

	i = 1;
	data->x = -1;
	exp->found = ZERO_INIT;
	while (data->nv[++data->x])
	{
		y = 0;
		i = 1;
		if (w[i] == '?')
			return (search_word(w, exp, data, state));
		if (w[i] == data->nv[data->x][y])
		{
			i = search_word(w, exp, data, state);
			if (exp->found == 1)
				return (i);
		}
	}
	if (exp->found == 0)
		return (word_not_found(w, state));
	return (i);
}
