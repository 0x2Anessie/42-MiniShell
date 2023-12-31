
#include "../../include/minishell.h"

//fonction qui initialise len i j et is_escaped dans la structure data
void	initialize_data(t_stringdata *data, char *str)
{
	data->len = ft_strlen(str);
	data->i = 0;
	data->j = 0;
	data->is_escaped = 0;
}

/*
	 conçue pour extraire le contenu d'une chaîne de caractères entre guillemets
	 simples ('), en tenant compte des caractères échappés. Elle parcourt la
	 chaîne et copie les caractères dans la même chaîne en éliminant les guillemets
	  simples et les caractères d'échappement.
*/
void	extract_within_quotes(t_stringdata *data, char *str)
{
	while (data->i < data->len && (str[data->i] != '\'' || data->is_escaped))
	{
		if (str[data->i] == '\\' && !data->is_escaped)
			data->is_escaped = 1;
		else
		{
			str[data->j] = str[data->i];
			data->j++;
		}
		data->i++;
		data->is_escaped = 0;
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
	while (data->i < data->len && (str[data->i] != quote || data->is_escaped))
	{
		if (str[data->i] == '\\' && !data->is_escaped)
			data->is_escaped = 1;
		else
		{
			str[data->j] = str[data->i];
			data->j++;
		}
		data->i++;
		data->is_escaped = 0;
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
	while (data.i < data.len)
	{
		if (str[data.i] == '\'' && !data.is_escaped)
		{
			data.i++;
			extract_within_quotes(&data, str);
			data.i++;
		}
		else
		{
			str[data.j] = str[data.i];
			data.i++;
			data.j++;
		}
	}
	str[data.j] = '\0';
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
	while (data.i < data.len)
	{
		if (str[data.i] == '\"' && !data.is_escaped)
		{
			data.i++;
			extract_quotes(&data, str, '\"');
			data.i++;
		}
		else
		{
			str[data.j] = str[data.i];
			data.i++;
			data.j++;
		}
	}
	str[data.j] = '\0';
}
