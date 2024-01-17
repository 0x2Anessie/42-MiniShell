#include "../../include/minishell.h"

/*
	Cette fonction crée un nouveau nœud pour une liste chaînée de
	variables d'environnement (t_env)
	Création et Allocation : Elle alloue de la mémoire pour un nouveau
	nœud t_env.
	Initialisation : Le contenu du nœud (node->content) est initialisé
	avec la chaîne de caractères str fournie, et le pointeur next est
	initialisé à NULL.
	Retour : Retourne le nouveau nœud. Si l'allocation échoue,
	retourne NULL.
*/
t_env	*create_node_str(char *str, t_data *data)
{
	t_env	*node;

	node = ft_malloc_with_tracking(data, sizeof(t_env));
	if (!node)
		return (NULL);
	node->var_env_name_and_value = str;
	node->next_var_env_name_and_value = NULL;
	return (node);
}

/*
	cette fonction regarde si le noeud head est NULL, si cest le cas
	elle cree un noeud et le defini comme le premier noeud
	si la list contient deja des elem, elle parcou la list jusqu'au
	dernier elem puis cree un nouveau noeud avec str en utilissant
	create_node_str et l'ajoute a la fin de la liste
*/
void	lst_add_back(t_exec *utils, char *str, t_data *data)
{
	t_env	*current;
	t_env	*head;

	head = utils->head_env_lst;
	if (head == NULL)
	{
		head = create_node_str(str, data);
	}
	else
	{
		current = head;
		while (current->next_var_env_name_and_value)
		{
			current = current->next_var_env_name_and_value;
		}
		current->next_var_env_name_and_value = create_node_str(str, data);
	}
}
