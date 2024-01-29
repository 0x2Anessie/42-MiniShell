/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:23:47 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/23 12:03:28 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Utilisé dans la fonction init_env_list_with_pwd_if_empty, il crée un
	nouveau noeud pour une variable OLDPWD.
*/
void	malloc_no_env_initial_node(t_data *data, char *str, t_env **env)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		perror(ERR_MEMORY_ALLOCATION);
		return ;
	}
	new_node->var_env_name_and_value = ft_strdup(data, str);
	new_node->next_var_env_name_and_value = NULL;
	*env = new_node;
}

/*
	Est utilisée dans le cas où aucune variable d'environnement n'est passée
	à Minishell. La fonction getwcd est utilisée pour trouver le répertoire
	de travail courant (PWD). Ce path sera mit dans une variable temporaire
	qui sera elle même mise dans un noeud renvoyé à create_env_list_from_array.
	Sans variables d'environnement, certaines fonctionnalités de base du
	shell pourraient ne pas fonctionner correctement.
*/
t_env	*init_env_list_with_pwd_if_empty(t_data *data, t_env *final)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	tmp = ft_strjoin_free_arg2_with_memory_tracking(ENV_SET_OLDPWD, tmp, data);
	malloc_no_env_initial_node(data, tmp, &final);
	return (final);
}

/*
	Initialise et configure une structure t_exec pour l'environnement
	d'exécution.
	Cette fonction alloue et prépare une structure contenant les informations
	essentielles pour la gestion de l'environnement d'exécution, y compris les
	variables d'environnement et les indicateurs d'état.
 */
t_exec	*init_env(t_data *data, char **env)
{
	t_exec	*utils;

	utils = malloc_track(data, sizeof(t_exec));
	if (!utils)
		return (NULL);
	utils->linked_list_full_env_var_copy_alpha = NULL;
	if (env)
		utils->linked_list_full_env_var_copy_alpha = \
		create_env_list_from_array(data, env);
	utils->head_env_lst = utils->linked_list_full_env_var_copy_alpha;
	utils->is_this_an_exec_in_heredoc = ZERO_INIT;
	return (utils);
}
