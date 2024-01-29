/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:30 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:32 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Cette fonction parcourt et affiche la liste des variables exporter
	elle affiche declare -x suivie de la valeur de la variable
	on utilise write fd quand on veux ecrire dans un fd special ou
	printf pour l'affiche standard
*/
void	print_export(t_export *head_of_linked_list_env_var, t_data *data)
{
	t_export	*current;

	current = head_of_linked_list_env_var;
	while (current != NULL)
	{
		if (data->utils->node->output_fd > 0)
		{
			ft_write_fd("declare -x ", data->utils->node->output_fd);
			ft_write_fd(current->env_var_name_and_value, \
			data->utils->node->output_fd);
			ft_write_fd("\n", data->utils->node->output_fd);
		}
		else if (!data->utils->node->output_redirection_error_id)
		{
			printf("declare -x ");
			printf("%s\n", current->env_var_name_and_value);
		}
		current = current->next_env_var_name_and_value;
	}
	current = head_of_linked_list_env_var;
	g_globi = 0;
}

//check la syntaxe de chaque mot grace a check_parsing_export 1 a 1
int	export_parsing_syntaxe(t_lexer *tmp, t_data *data)
{
	tmp = data->utils->head_lexer_lst;
	while (tmp)
	{
		if (check_parsing_export(tmp->cmd_segment) == 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
	Traite chaque argument passé à export et décide de l'ajouter ou non à la
	liste des variables exporter elle appel supp_quote_and_add_env pour traiter
	les caractere speciaux.
	Vérifie si la variable existe déjà et si elle doit être ajoutée à la liste
	des variables exportées. Utilise des fonctions comme
	verif_var_exist_export et lst_add_back_export pour cette gestion.
*/
void	process_word_and_add_export(t_lexer *tmp, t_data *data)
{
	supp_quote_and_add_env(&(data->utils), tmp, data);
	if (verif_var_exist_export(data->utils, tmp->cmd_segment, data) == 0
		&& verif_equal(tmp->cmd_segment, '=') == 0
		&& verif_var_exist_export_not_maj(data->utils, tmp->cmd_segment) == 0)
		lst_add_back_export(&(data->utils->head_of_linked_list_env_var), \
		tmp->cmd_segment, data);
	else if (check_case(tmp->cmd_segment)
		&& verif_var_exist_export(data->utils, tmp->cmd_segment, data) == 0)
	{
		tmp->cmd_segment = case_egale(tmp->cmd_segment, data);
		if (verif_var_exist_export(data->utils, tmp->cmd_segment, data) == 0)
			lst_add_back_export(&(data->utils->head_of_linked_list_env_var), \
			tmp->cmd_segment, data);
	}
	else if (verif_var_exist_export(data->utils, tmp->cmd_segment, data) == 0)
	{
		if (verif_var_exist_export(data->utils, tmp->cmd_segment, data) == 0
			&& verif_equal(tmp->cmd_segment, '=') == 1)
			lst_add_back_export(&(data->utils->head_of_linked_list_env_var), \
			tmp->cmd_segment, data);
		else if (verif_var_exist_export(\
		data->utils, tmp->cmd_segment, data) == 1)
			lst_add_back_export(&(data->utils->head_of_linked_list_env_var), \
			tmp->cmd_segment, data);
	}
}

/*
	gere les arg de export, elle appel export_parsing_syntaxe pour check la
	syntaxe puis pour chaque token use process_word_add_export si c'est un
	argument
*/
void	export_remaining(t_lexer *tmp, t_data *data)
{
	if (export_parsing_syntaxe(tmp, data))
		return ;
	tmp = data->utils->head_lexer_lst;
	while (tmp)
	{
		if (tmp->token == ARG)
			process_word_and_add_export(tmp, data);
		tmp = tmp->next;
	}
}

/*
	fonction principal de export
	elle commence par trier les variable exporter
	Flux de Contrôle Principal : Basé sur les conditions, décide soit d'afficher
	les variables exportées (si aucune nouvelle variable n'est ajoutée), soit de
	 traiter les nouveaux arguments d'exportation.
*/
int	export_things(t_lexer *lexer_lst, t_data *data)
{
	t_lexer	*tmp;

	sort_linked_list_env_var(&(data->utils->head_of_linked_list_env_var));
	tmp = lexer_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->cmd_segment, CMD_EXPORT_VARS) && \
		!(tmp->next && tmp->next->cmd_segment && \
		!(tmp->next->cmd_segment[0] == '\0')))
			print_export(data->utils->head_of_linked_list_env_var, data);
		else if (!ft_strcmp(tmp->cmd_segment, CMD_EXPORT_VARS) && tmp->next
			&& tmp->next->token != ARG)
			print_export(data->utils->head_of_linked_list_env_var, data);
		else if ((ft_strcmp(tmp->cmd_segment, CMD_EXPORT_VARS) == 0) \
		&& (tmp->next && tmp->next->token == ARG))
		{
			tmp = tmp->next;
			export_remaining(tmp, data);
			break ;
		}
		tmp = tmp->next;
	}
	return (0);
}
