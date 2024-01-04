/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:32:12 by raveriss          #+#    #+#             */
/*   Updated: 2024/01/02 00:21:50 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

unsigned int	g_signal_received = ZERO_INIT;

/**
 * @nom: init_data
 * @brief: Initialise la structure de données principale du programme.
 *
 * @description:
 * 'init_data' est responsable de l'initialisation de la structure 't_data'
 * , qui contient toutes les données principales utilisées par le programme.
 * Cette initialisation est cruciale pour préparer l'environnement
 * d'exécution et assurer que toutes les données nécessaires sont correctement
 * configurées dès le début.
 *
 * @parametres:
 *   - data: t_data &data, pointeur vers la structure de données à initialiser.
 *   - ac: int ac, le nombre d'arguments de la ligne de commande.
 *   - av: char **av, les arguments de la ligne de commande.
 *   - env: char **env, les variables d'environnement.
 *
 * @pourquoi: 
 *   Préparation de l'environnement d'exécution : 'init_data' s'assure que
 * toutes les composantes nécessaires du programme sont configurées et prêtes
 * à être utilisées. Cela inclut la configuration des arguments de la ligne
 * de commande, des variables d'environnement, et l'initialisation des
 * structures internes.
 *
 * @valeur_de_retour: 
 *   Aucune (void). La fonction initialise la structure 'data' en place.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - La fonction ne génère pas d'erreur mais repose sur la validité des
 * pointeurs fournis.
 *
 * @exemples_utilisation:
 *   t_data *data;
 *   init_data(&data, argc, argv, environ);
 *
 * @dependances: Aucune.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Assigner 'ac', 'av', 'env' à leurs champs respectifs dans 'data'
 *     |
 *     v
 *   Initialiser les champs 'line', 'allcommand', 'envpaths', 'lexer_list' à NULL
 *     |
 *     v
 *   Initialiser 'index' dans 'data' à ZERO_INIT
 *     |
 *     v
 *   Fin
 */
void	init_data(t_data *data, int ac, char **av, char **env)
{
	data->ac = ac;
	data->av = av;
	data->env = env;
	data->trash_memory = NULL;
	data->utils = NULL;
	data->line = NULL;
	data->allcommand = NULL;
	data->envpaths = NULL;
	data->lexer_list = NULL;
	data->index = ZERO_INIT;
}

/**
 * @nom: exit_error
 * @brief: Affiche un message d'erreur et termine le programme.
 *
 * @description:
 *   'exit_error' est utilisée pour gérer les situations d'erreur fatales où
 * le programme ne peut pas continuer son exécution. Elle écrit un message
 * d'erreur sur STDERR et termine le programme avec un statut d'erreur.
 *
 * @parametres:
 *   - str: char *str, le message d'erreur à afficher.
 *
 * @pourquoi: 
 *   La gestion d'erreurs critiques : Dans certains cas, il est nécessaire
 * d'arrêter immédiatement le programme pour éviter des comportements
 * imprévisibles ou des dommages plus graves. 'exit_error' assure une sortie
 * claire et informative en cas de telles situations critiques.
 *
 * @valeur_de_retour: 
 *   Aucune. La fonction termine le programme avec exit(1).
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Cette fonction arrête l'exécution du programme, donc toutes les
 * ressources non libérées ou tous les processus fils non gérés resteront en
 * état.
 *
 * @exemples_utilisation:
 *   if (condition_erreur)
 *       exit_error("Erreur: condition non remplie");
 *
 * @dependances: 
 *   - 'ft_write_fd' pour écrire le message d'erreur.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Écrire le message d'erreur sur STDERR (ft_write_fd)
 *     |
 *     v
 *   Terminer le programme (exit(1))
 *     |
 *     v
 *   Fin
 */
void	exit_error(char *str)
{
	ft_write_fd(str, STDERR_FILENO);
	exit(1);
}

/**
 * @nom: get_new_env
 * @brief: Construit un tableau de chaînes de caractères à partir d'une liste
 * chaînée d'environnement.
 *
 * @description:
 * 'get_new_env' parcourt la liste chaînée 'env_lst' représentant les
 * variables d'environnement et construit un tableau de chaînes de caractères.
 * Ce tableau est utilisé pour passer l'environnement
 *   aux processus enfants créés par le shell.
 *
 * @parametres:
 *   - env_lst: t_env *env_lst, la liste chaînée des variables d'environnement.
 *
 * @valeur_de_retour: 
 *  Renvoie un tableau de chaînes de caractères représentant l'environnement.
 * Retourne NULL en cas d'échec d'allocation.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Retourne NULL si l'allocation de mémoire pour 'new_env' échoue.
 *
 * @exemples_utilisation:
 *   t_env *env_lst = build_env_list(envp);
 *   char **new_env = get_new_env(data, env_lst);
 *
 * @dependances: 
 *   - 'ft_malloc_with_tracking' pour l'allocation mémoire avec suivi.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Initialiser 'i' à ZERO_INIT et conserver la tête de 'env_lst'
 *     |
 *     v
 *   Parcourir 'env_lst' pour compter les éléments
 *     |
 *     v
 *   Allocation de 'new_env' avec la taille calculée
 *     |
 *     v
 *   'new_env' est-il alloué ?
 *  /       \
 * OUI      NON
 *  |        |
 *  v        v
 * Initialiser  Retourner NULL
 * 'new_env[i]' à NULL
 *  |
 *  v
 * Revenir à la tête de 'env_lst'
 *  |
 *  v
 * Parcourir 'env_lst' pour remplir 'new_env'
 *  |
 *  v
 * Fin
 */
char	**get_new_env(t_data *data, t_env *env_lst)
{
	char	**new_env;
	int		i;
	t_env	*head;

	head = env_lst;
	i = ZERO_INIT;
	while (env_lst)
	{
		i++;
		env_lst = env_lst->next;
	}
	new_env = ft_malloc_with_tracking(data, sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	new_env[i] = NULL;
	env_lst = head;
	i = 0;
	while (env_lst)
	{
		new_env[i++] = env_lst->content;
		env_lst = env_lst->next;
	}
	env_lst = head;
	return (new_env);
}

/**
 * @nom: prompt_loop
 * @brief: Boucle principale de traitement des commandes dans le shell minishell.
 *
 * @description:
 *   'prompt_loop' traite la ligne de commande saisie par l'utilisateur,
 * effectuant l'analyse lexicale, syntaxique, l'expansion des variables, et
 * enfin l'exécution des commandes.
 * 
 * @pourquoi:
 * - Gestion de Commande : 'prompt_loop' est essentielle pour gérer le
 * cycle de vie complet d'une commande dans le shell, de la saisie par
 * l'utilisateur jusqu'à son exécution. 
 *   - Flexibilité et Robustesse : En traitant chaque étape séparément
 * (analyse lexicale, syntaxique, expansion, exécution), 
 *     cette fonction offre une flexibilité et une robustesse accrues,
 * permettant une meilleure gestion des erreurs et une 
 *     plus grande adaptabilité aux différentes commandes.
 *   - Interactivité Utilisateur : Elle permet une interaction dynamique avec
 * l'utilisateur, en traitant les commandes saisies et en fournissant un
 * retour en cas d'erreur, contribuant à une expérience utilisateur cohérente
 * et intuitive.
 *   - Expansion et Exécution : La prise en charge de l'expansion des
 * variables et l'exécution des commandes permet au shell de fonctionner comme
 * un interpréteur de commandes complet, capable de gérer des scénarios
 * complexes et des chaînes de commandes variées.
 *
 * @parametres:
 *   - tmp: char *tmp, ligne de commande saisie par l'utilisateur.
 *   - data: t_data &data, structure contenant les informations et les listes
 * pour le traitement de la commande.
 *   - env: char **env, environnement du programme sous forme de tableau de
 * chaînes de caractères.
 *
 * @valeur_de_retour: 
 *   Aucune (void). La fonction modifie les structures et variables en place.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Termine le programme si 'tmp' est NULL.
 *   - Met à jour 'globi' en cas d'erreur de parsing.
 *
 * @exemples_utilisation:
 *   char *command = readline("minishell$ ");
 *   t_data *data;
 *   prompt_loop(command, data, env);
 *
 * @dependances: 
 *   - 'ft_init_lexer_process' pour l'analyse lexicale.
 *   - 'ft_parser' pour l'analyse syntaxique.
 *   - 'get_new_env' pour obtenir l'environnement actuel.
 *   - 'expand' pour l'expansion des variables.
 *   - 'ft_init_exec' et 'manage_execution_resources' pour la préparation et
 * l'exécution des
 * commandes.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   'tmp' est-il NULL ?
 *  /        \
 * OUI       NON
 *  |         |
 *  v         v
 * Sortie    'tmp' est-il non vide ?
 *           /       \
 *         OUI      NON
 *          |         |
 *          v         v
 *        Analyse   Fin de la fonction
 *        lexicale
 *          |
 *          v
 *        Analyse syntaxique réussie ?
 *       /        \
 *     OUI       NON
 *      |         |
 *      v         v
 *    Expansion Mise à jour de 'globi', 
 *    des variables  fin de la fonction
 *      |
 *      v
 *    'tmp_lex' et 'tmp_lex->word' sont-ils non NULL ?
 *   /        \
 * OUI       NON
 *  |         |
 *  v         v
 * Initialisation  Fin de la fonction
 * et préparation 
 * pour exécution
 */
void	prompt_loop(char *tmp, t_data *data, char **env)
{
	t_lexer	*tmp_lex;
	char	**new_env;

	(void)env;
	if (!tmp)
		exit_all(data);
	if (tmp && tmp[0])
	{
		data->line = tmp;
		ft_init_lexer_process(data);
		if (!ft_parser(data))
		{
			g_signal_received = 2;
			return ;
		}
		tmp_lex = data->lexer_list;
		new_env = get_new_env(data, data->utils->env_lst);
		expand(data->quote, new_env, tmp_lex, data);
		if (tmp_lex && tmp_lex->word)
		{
			ft_init_exec(data);
			manage_execution_resources(data);
		}
	}
}

/**
 * @nom: main
 * @brief: Point d'entrée principal pour le shell minishell.
 *
 * @description:
 *   La fonction 'main' est le point d'entrée du shell minishell. Elle
 * initialise les structures de données, 
 *   configure l'environnement, et entre dans une boucle infinie pour lire et
 * traiter les commandes saisies par l'utilisateur.
 *
 * @parametres:
 *   - ac: int ac, le nombre d'arguments passés au programme.
 *   - av: char **av, tableau des arguments passés au programme.
 *   - env: char **env, environnement du programme sous forme de tableau de
 * chaînes de caractères.
 *
 * @pourquoi:
 *   - Initialisation et Exécution : Met en place les structures et
 * l'environnement nécessaires, puis gère l'interaction utilisateur pour
 * l'exécution des commandes dans le shell.
 *   - Gestion des Entrées : Permet de lire et d'ajouter les commandes saisies
 * à l'historique, contribuant à une expérience utilisateur interactive.
 *   - Boucle de Commande : Maintient le shell en fonctionnement continu,
 * traitant les commandes saisies jusqu'à ce que l'utilisateur décide de
 * quitter.
 *
 * @valeur_de_retour: 
 *   Cette fonction ne retourne normalement pas, mais peut terminer le
 * programme en cas d'erreur initiale.
 *
 * @erreurs_possibles_et_effets_de_bord: 
 *   - Termine le programme si le nombre d'arguments est incorrect ou en cas
 * d'erreur d'initialisation.
 *
 * @exemples_utilisation:
 *   int main(int argc, char **argv, char **envp)
 *   {
 *       // Exécution du shell minishell
 *   }
 *
 * @dependances: 
 *   - 'init_data' pour initialiser les structures de données.
 *   - 'init_env' pour configurer l'environnement.
 *   - 'readline' et 'add_history' pour la gestion des entrées utilisateur.
 *   - 'prompt_loop' pour le traitement des commandes saisies.
 *   - 'handle_sig' pour la gestion des signaux.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Vérifier le nombre d'arguments (ac)
 *  /      \
 * OUI     NON
 *  |       |
 *  v       v
 * Erreur,  Initialiser les données 
 * sortie   et l'environnement
 *  |       |
 *  \-------/
 *     |
 *     v
 *   Entrer dans la boucle infinie (INFINITY_LOOP)
 *     |
 *     v
 *   Libérer 'tmp' si non NULL
 *     |
 *     v
 *   Gérer les signaux (handle_sig)
 *     |
 *     v
 *   Lire une ligne de commande (readline)
 *     |
 *     v
 *   Ajouter la ligne à l'historique (add_history)
 *     |
 *     v
 *   Traiter la ligne dans la boucle de prompt (prompt_loop)
 *     |
 *     v
 *   Répéter la boucle
 */
int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*tmp;

	printf(WELCOME_MSG);
	if (ac != 1)
		exit_error("bad number of arguments");
	init_data(&data, ac, av, env);
	data.utils = init_env(&data, env);
	tmp = NULL;
	data.utils->export_lst = NULL;
	while (42)
	{
		free(tmp);
		tmp = NULL;
		handle_sig(&data);
		tmp = readline("minishell$ ");
		add_history(tmp);
		prompt_loop(tmp, &data, env);
	}
}
