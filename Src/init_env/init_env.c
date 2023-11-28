
// env est determiné par le 3e argument du main "char **env", qui permet 
// d'accéder aux variables globales définies par le système d'exploitation.

#include "../../include/minishell.h"

/*
	Utilisé dans la fonction if_no_env, il crée un nouveau noeud pour
	une variable OLDPWD.
*/
void	no_env_node_init(char *str, t_env **env)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		perror("Erreur lors de l'allocation de mémoire");
		return ;
	}
	new_node->content = ft_strdup(str);
	new_node->next = NULL;
	*env = new_node;
}

/*
	Est utilisée dans le cas où aucune variable d'environnement n'est passée
	à Minishell. La fonction getwcd est utilisée pour trouver le répertoire
	de travail courant (PWD). Ce path sera mit dans une variable temporaire
	qui sera elle même mise dans un noeud renvoyé à init_env_list.
	Sans variables d'environnement, certaines fonctionnalités de base du 
	shell pourraient ne pas fonctionner correctement.
*/
t_env	*if_no_env(t_env *final)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	tmp = ft_strjoin_2("OLDPWD=", tmp);
	no_env_node_init(tmp, &final);
	return (final);
}

/*
	La liste chaînée d'environnement est stockée dans Utils.
	Elle initialise env_lst et head_env_lst avec la liste
	qu'on a créé puis initialise le code d'erreur, le heredoc
	et ?ret? à 0.
*/
t_exec	*init_env(char **env)
{
	t_exec	*utils;

	utils = ft_malloc(sizeof(t_exec));
	if (!utils)
		return (NULL);
	utils->env_lst = NULL;
	if (env)
		utils->env_lst = init_env_list(env);
	utils->head_env_lst = utils->env_lst;
	utils->err = 0;
	utils->ret = 0;
	utils->is_here_doc = 0;
	return (utils);
}
