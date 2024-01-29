/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:20:19 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:20:20 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//fonction qui initialise len i j et is_escaped dans la structure data
void	initialize_data(t_stringdata *data, char *str)
{
	data->word_len_of_cmd_with_quot_includ = ft_strlen(str);
	data->indx_quote_escape_handling = ZERO_INIT;
	data->indx_rebuilt_str_excl_quotes = ZERO_INIT;
	data->is_current_char_preceded_by_escape_char = ZERO_INIT;
}

/*
	 conçue pour extraire le contenu d'une chaîne de caractères entre guillemets
	 simples ('), en tenant compte des caractères échappés. Elle parcourt la
	 chaîne et copie les caractères dans la même chaîne en éliminant les
	 guillemets simples et les caractères d'échappement.
*/
void	extract_within_quotes(t_stringdata *data, char *str)
{
	while (data->indx_quote_escape_handling < \
	data->word_len_of_cmd_with_quot_includ && \
	(str[data->indx_quote_escape_handling] != '\'' \
	|| data->is_current_char_preceded_by_escape_char))
	{
		if (str[data->indx_quote_escape_handling] == '\\' \
		&& !data->is_current_char_preceded_by_escape_char)
			data->is_current_char_preceded_by_escape_char = 1;
		else
		{
			str[data->indx_rebuilt_str_excl_quotes] = \
			str[data->indx_quote_escape_handling];
			data->indx_rebuilt_str_excl_quotes++;
		}
		data->indx_quote_escape_handling++;
		data->is_current_char_preceded_by_escape_char = 0;
	}
}

/*
	Similaire à extract_within_quotes, mais pour un type de guillemet spécifique
	(simple ou double). Cette fonction est plus générique car elle prend un
	caractère quote comme paramètre, ce qui signifie qu'elle peut être utilisée
	 pour des guillemets simples ou doubles.
*/
void	extract_quotes(t_stringdata *data, char *str, char quote)
{
	while (data->indx_quote_escape_handling < \
	data->word_len_of_cmd_with_quot_includ && \
	(str[data->indx_quote_escape_handling] != \
	quote || data->is_current_char_preceded_by_escape_char))
	{
		if (str[data->indx_quote_escape_handling] == '\\' \
		&& !data->is_current_char_preceded_by_escape_char)
			data->is_current_char_preceded_by_escape_char = 1;
		else
		{
			str[data->indx_rebuilt_str_excl_quotes] = \
			str[data->indx_quote_escape_handling];
			data->indx_rebuilt_str_excl_quotes++;
		}
		data->indx_quote_escape_handling++;
		data->is_current_char_preceded_by_escape_char = 0;
	}
}

/*
	Elle initialise les données de la chaîne, puis parcourt la chaîne
	en retirant les guillemets simples. Si elle rencontre un guillemet
	 simple non échappé, elle appelle extract_within_quotes pour traiter
	  le contenu entre guillemets.
*/
void	remove_single_quotes(char *str)
{
	t_stringdata	data;

	initialize_data(&data, str);
	while (data.indx_quote_escape_handling < \
	data.word_len_of_cmd_with_quot_includ)
	{
		if (str[data.indx_quote_escape_handling] == '\'' \
		&& !data.is_current_char_preceded_by_escape_char)
		{
			data.indx_quote_escape_handling++;
			extract_within_quotes(&data, str);
			data.indx_quote_escape_handling++;
		}
		else
		{
			str[data.indx_rebuilt_str_excl_quotes] = \
			str[data.indx_quote_escape_handling];
			data.indx_quote_escape_handling++;
			data.indx_rebuilt_str_excl_quotes++;
		}
	}
	str[data.indx_rebuilt_str_excl_quotes] = '\0';
}

/*
	Fonctionne de manière similaire à remove_single_quotes, mais pour les
	guillemets doubles. Elle utilise extract_quotes pour gérer les chaînes
	entre guillemets doubles.
*/
void	remove_double_quotes(char *str)
{
	t_stringdata	data;

	initialize_data(&data, str);
	while (data.indx_quote_escape_handling < \
	data.word_len_of_cmd_with_quot_includ)
	{
		if (str[data.indx_quote_escape_handling] == '\"' \
		&& !data.is_current_char_preceded_by_escape_char)
		{
			data.indx_quote_escape_handling++;
			extract_quotes(&data, str, '\"');
			data.indx_quote_escape_handling++;
		}
		else
		{
			str[data.indx_rebuilt_str_excl_quotes] = \
			str[data.indx_quote_escape_handling];
			data.indx_quote_escape_handling++;
			data.indx_rebuilt_str_excl_quotes++;
		}
	}
	str[data.indx_rebuilt_str_excl_quotes] = '\0';
}
