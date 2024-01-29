/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_checking.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:04 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:05 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	cette fonction gere les guillemets dans les valeurs des variables
	 exporter elle va passer sur chaque variable de la liste data
	Utilise check_case et check_case_with_char pour déterminer si des
	modifications sont nécessaires, puis applique case_egale pour ajuster
	la valeur de la variable si nécessaire.
*/
void	export_quotes(t_export *don, t_data *data)
{
	t_export	*tmp;

	tmp = don;
	while (tmp)
	{
		if (check_case(tmp->env_var_name_and_value) && \
		check_case_with_char(tmp->env_var_name_and_value))
			tmp->env_var_name_and_value = case_egale(\
			tmp->env_var_name_and_value, data);
		tmp = tmp->next_env_var_name_and_value;
	}
	tmp = don;
}

/*
	passe sur chaque elem de str et retourne 1 si elle trouve un = suivie direct
	d'un autre caractere , indiquant qu'il s'agit d'une assignation de variable
*/
int	check_case_with_char(char *str)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
	{
		if (str[i] == '=' && str[i + 1] != '\0')
			return (1);
		i++;
	}
	return (0);
}

//comme celle d'avant mais retourn 1 seulement si elle trouve un =
int	check_case(char *str)
{
	int	i;

	i = ZERO_INIT;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

/*
Traite un mot (argument) en enlevant les guillemets et vérifie
s'il doit être ajouté aux variables d'environnement.
Suppression des Guillemets : Appelle remove_double_quotes et remove_single_quotes
pour traiter les guillemets dans tmp->word.
Ajout aux Variables d'Environnement : Si la chaîne contient un '=', et si la
variable n'existe pas déjà (verif_var_exist), elle est ajoutée à la liste des
variables d'environnement avec lst_add_back.
*/
void	supp_quote_and_add_env(t_exec **utils, t_lexer *tmp, t_data *data)
{
	remove_double_quotes(tmp->cmd_segment);
	remove_single_quotes(tmp->cmd_segment);
	if (verif_equal(tmp->cmd_segment, '=') && \
	verif_var_exist(*utils, tmp->cmd_segment) == 0)
	{
		lst_add_back(*utils, tmp->cmd_segment, data);
	}
}
