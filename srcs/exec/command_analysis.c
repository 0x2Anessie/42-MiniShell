#include "../../include/minishell.h"

/**
 * @nom: is_exact_command_match
 *
 * @description:
 * Compare un mot de la liste de lexèmes avec une commande spécifiée pour
 * vérifier si elles sont identiques en termes de contenu et de longueur. Cette
 * fonction est essentielle pour identifier des commandes spécifiques dans un
 * interpréteur de commandes ou un shell, assurant ainsi que les commandes
 * entrées correspondent exactement à celles attendues, ni plus courtes ni plus
 * longues.
 *
 * @parametres:
 * - lexer_lst: Un pointeur vers t_lexer, représentant le lexème courant à
 * comparer.
 * - command: Une chaîne de caractères constante représentant la commande à
 * vérifier.
 * - command_length: Un entier indiquant la longueur de la commande à vérifier.
 *
 * @fonctionnement:
 * - Utilise ft_strncmp pour comparer le mot du lexème (lexer_lst->word) avec
 * la commande donnée sur les 'command_length' premiers caractères.
 * - Utilise strlen2 pour vérifier que la longueur du mot du lexème est
 * exactement égale à 'command_length'.
 * - Retourne 1 (vrai) si les deux conditions sont remplies, sinon 0 (faux).
 *
 * @valeur_de_retour:
 * - Retourne 1 si le mot du lexème est identique à la commande spécifiée en
 * termes de contenu et de longueur.
 * - Retourne 0 dans le cas contraire.
 *
 * @erreurs_et_effets_de_bord:
 * - Si lexer_lst ou lexer_lst->word est NULL, le comportement est indéterminé.
 * - La fonction dépend de la validité de 'lexer_lst' et 'command'.
 *
 * @exemples_utilisation:
 * - is_exact_command_match(lexeme, "exit", 4) retournera 1 si lexeme->word est
 * "exit".
 * - is_exact_command_match(lexeme, "echo", 4) retournera 0 si lexeme->word est
 * "echoo".
 *
 * @dependances:
 * - ft_strncmp: Compare les chaînes de caractères avec une limite de longueur.
 * - strlen2: Calcule la longueur d'une chaîne de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   ft_strncmp(lexer_lst->word, command, command_length) == 0 et 
 *   strlen2(lexer_lst->word) == command_length ?
 *     /        \
 *   VRAI      FAUX
 *     |          \
 *     |           \
 *     v            v
 *   Retourner 1   Retourner 0
 */
int	is_exact_command_match(\
t_lexer *lexer_lst, const char *command, int command_length)
{
	return (!ft_strncmp(lexer_lst->word, command, command_length) \
	&& strlen2(lexer_lst->word) == command_length);
}

/**
 * @fonction: is_built_in_command
 * @breve_description: Détermine si un mot dans une liste de lexèmes
 * représente une commande intégrée.
 * 
 * @param lexer_lst: Pointeur sur une structure t_lexer représentant la liste
 * des lexèmes.
 * 
 * @description_detaillee:
 * La fonction 'is_built_in_command' est conçue pour identifier si le mot
 * courant dans la liste des lexèmes ('lexer_lst') correspond à l'une des
 * commandes intégrées du shell. Elle effectue cette vérification en utilisant
 * une série de comparaisons avec les noms des commandes intégrées définies
 * (comme 'pwd', 'echo', etc.).
 *   
 * Pourquoi cette vérification est-elle nécessaire ?
 * - Filtrage des commandes : Dans un shell, il est crucial de distinguer les
 * commandes intégrées des commandes systèmes externes. Cela permet au shell de
 * les traiter correctement, en exécutant les commandes intégrées directement
 * et en déléguant les autres au système.
 * - Optimisation de l'exécution : Les commandes intégrées sont généralement
 * exécutées plus rapidement car elles ne nécessitent pas de créer un processus
 * séparé.
 * - Respect de la logique du shell : Certaines commandes intégrées, comme
 * 'cd', doivent être exécutées dans le contexte du shell actuel pour être
 * efficaces, car elles modifient l'état du shell lui-même.
 *
 * @valeur_de_retour: 
 * Retourne 1 (vrai) si le mot correspond à une commande intégrée, 0 (faux)
 * sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'lexer_lst' est NULL ou si 'lexer_lst->word' est NULL, la fonction
 * retourne 0 sans effectuer de comparaisons.
 * - La fonction dépend des commandes intégrées définies et de leur
 * correspondance exacte avec les mots dans 'lexer_lst'.
 *
 * @exemples_utilisation:
 * t_lexer lexer;
 * lexer.word = "echo";
 * int result = is_built_in_command(&lexer);
 * -------> Résultat attendu : 1
 *
 * @dependances: 
 * - is_exact_command_match() : Utilisée pour comparer le mot actuel avec chaque
 * commande intégrée.
 * - CMD_PRINT_DIRCT, CMD_ECHO, etc. : Constantes représentant les noms des
 * commandes intégrées.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   lexer_lst->word est NULL ?
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * Retourner 0  Comparer le mot avec les commandes intégrées
 *              (PWD, ECHO, ENV, UNSET, CD, EXPORT, EXIT)
 *              |
 *              v
 *            Retourner 1 si une correspondance est trouvée, sinon 0
 */
int	is_built_in_command(t_lexer *lexer_lst)
{
	if (lexer_lst->word == NULL)
		return (0);
	return (\
	is_exact_command_match(\
	lexer_lst, CMD_PRINT_DIRCT, ft_strlen(CMD_PRINT_DIRCT)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_ECHO, ft_strlen(CMD_ECHO)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_ENV_VARS, ft_strlen(CMD_ENV_VARS)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_UNSET_VARS, ft_strlen(CMD_UNSET_VARS)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_CHANG_DIRCT, ft_strlen(CMD_CHANG_DIRCT)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_EXPORT_VARS, ft_strlen(CMD_EXPORT_VARS)) \
	|| is_exact_command_match(\
	lexer_lst, CMD_EXIT_SHELL, ft_strlen(CMD_EXIT_SHELL)));
}

/**
 * @fonction: should_continue_execution
 * @breve_description: Détermine si l'exécution doit se poursuivre en fonction
 * de l'état actuel.
 * 
 * @paramdata: Pointeur sur la structure t_all représentant l'état global du
 * programme.
 * @param y: Pointeur sur un entier utilisé dans la logique de contrôle de
 * l'exécution.
 * 
 * @description_detaillee:
 * La fonction 'should_continue_execution' évalue plusieurs conditions pour
 * décider si le processus d'exécution doit se poursuivre. Elle vérifie
 * l'existence et l'état de 'node' dans data->utils', si 'node' a une commande 
 * à exécuter ('has_cmd'), et si 'can_run' dans data->utils' est vrai.
 * De plus, elle incrémente la valeur pointée par 'y[1]' et vérifie si elle est
 * supérieure ou égale à 0.
 *   
 * Pourquoi cette vérification multi-conditionnelle ?
 * - Contrôle précis de l'exécution : Chacune de ces conditions joue un rôle
 * clé dans la décision de continuer ou d'arrêter l'exécution, permettant une
 * gestion fine du flux de contrôle.
 * - Gestion de l'état du programme : La vérification de l'état de différents
 * composants de data' assure que l'exécution se poursuit uniquement si l'état
 * du programme le permet.
 *
 * @valeur_de_retour: 
 * Retourne 1 (vrai) si toutes les conditions sont remplies pour continuer
 * l'exécution, 0 (faux) sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si data' ou data->utils->node' est NULL, la fonction retournera 0.
 * - La modification de 'y[1]' a un effet de bord et doit être prise en compte
 * dans le flux de contrôle.
 *
 * @exemples_utilisation:
 * t_all all;
 * int y[2] = {0, 0};
 * int result = should_continue_execution(&all, y);
 * -------> Résultat attendu : 1 ou 0
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *  data->utils->node existe etdata->utils->node->has_cmd == 1 ?
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * y[1]++ >= 0 etdata->utils->can_run ?  Retourner 0
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * Retourner 1  Retourner 0
 */
int	should_continue_execution(t_data *data, int *y)
{
	return (data->utils->node \
	&& data->utils->node->has_cmd == 1 && ++y[1] >= 0 && data->utils->can_run);/*         ---> condition non intelligible --> fonction         */
}

/**
 * @fonction: check_redirection_validity_in_node
 * @breve_description: Évalue la validité d'une redirection pour un nœud donné.
 * 
 * @param node: Pointeur sur une structure t_node représentant le nœud à
 * évaluer.
 * 
 * @description_detaillee:
 * La fonction 'check_redirection_validity_in_node' a pour but de déterminer si
 * les paramètres de redirection d'un nœud spécifié ('node') sont valides.
 * Cette évaluation se base sur deux critères :
 * - La validité de l'entrée ('input_fd') du nœud, qui ne doit pas être égale à
 * INVALID_PIPE.
 * - La validité de la sortie en cas d'échec ('out_fail'), qui ne doit pas être
 * égale à OUT_FAIL.
 *   
 * Pourquoi ces critères sont-ils importants ?
 * - Gestion des erreurs de redirection : Ces vérifications permettent de
 * s'assurer que les redirections d'entrée et de sortie sont configurées
 * correctement et peuvent être utilisées sans erreur.
 * - Fiabilité du flux de données : En confirmant la validité des redirections,
 * on garantit un traitement correct des données entre les différents
 * composants du système.
 *
 * @valeur_de_retour: 
 * Retourne 1 (vrai) si les conditions de validité sont remplies, 0 (faux)
 * sinon.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'node' est NULL, la fonction retournera 0, car elle ne peut pas
 * vérifier la validité des redirections.
 * - La fonction se repose sur les valeurs prédéfinies de INVALID_PIPE et
 * OUT_FAIL pour évaluer la validité.
 *
 * @exemples_utilisation:
 * t_node node;
 * node.input_fd = 1;
 * node.out_fail = 0;
 * int result = check_redirection_validity_in_node(&node);
 * -------> Résultat attendu : 1
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   node->input_fd != INVALID_PIPE et node->out_fail != OUT_FAIL ?
 *  /        \
 * VRAI      FAUX
 *  |         \
 *  |          \
 *  v           v
 * Retourner 1  Retourner 0
 */
int	check_redirection_validity_in_node(t_node *node)
{
	return (node->input_fd != INVALID_PIPE && node->out_fail != OUT_FAIL);/*         ---> condition non intelligible --> fonction         */
}

/**
 * @fonction: find_next_command_in_lexer
 * @breve_description: Trouve la prochaine commande dans une liste de lexèmes.
 * 
 * @param lexer_list: Pointeur sur t_lexer, la liste de lexèmes à parcourir.
 * 
 * @description_detaillee:
 * La fonction 'find_next_command_in_lexer' parcourt une liste de lexèmes fournie
 * ('lexer_list') pour trouver le premier lexème dont le token est CMD
 * (commande). Elle avance dans la liste en vérifiant chaque élément jusqu'à ce
 * qu'elle trouve un lexème avec un token de commande ou atteigne la fin de la
 * liste.
 *   
 * Pourquoi cette recherche est-elle nécessaire ?
 * - Identification des commandes : Dans un interpréteur de commandes, il est
 * essentiel d'identifier les commandes distinctes pour les traiter
 * correctement. Cette fonction facilite la segmentation et l'analyse des
 * commandes entrées par l'utilisateur.
 * - Préparation à l'exécution : En isolant les commandes, la fonction aide à
 * préparer le système pour exécuter chaque commande de manière ordonnée et
 * structurée.
 *
 * @valeur_de_retour: 
 * Retourne un pointeur sur le premier lexème avec un token CMD trouvé, ou NULL
 * si aucune commande n'est trouvée.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 * - Si 'lexer_list' est NULL dès le départ, la fonction retournera
 * immédiatement NULL.
 * - La fonction ne modifie pas la liste originale mais renvoie un pointeur
 * vers un élément dans celle-ci.
 *
 * @exemples_utilisation:
 * t_lexer *lexer_list = initialisé avec des lexèmes ;
 * t_lexer *next_cmd = find_next_command_in_lexer(lexer_list);
 * -------> Résultat attendu : pointeur vers le prochain CMD
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Parcourir lexer_list
 * jusqu'à trouver un lexème avec
 * token == CMD ou atteindre la fin
 *     |
 *     v
 *   Retourner le pointeur 
 *   sur le lexème trouvé ou NULL 
 *   si aucun CMD n'est trouvé
 */
t_lexer	*find_next_command_in_lexer(t_lexer *lexer_list)
{
	while (lexer_list && lexer_list->token != CMD)/*         ---> condition non intelligible --> fonction         */
		lexer_list = lexer_list->next;
	return (lexer_list);
}
