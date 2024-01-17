#include "../../include/minishell.h"

bool	is_matching_env_var_name(\
char *w, t_data *data, int i, int env_var_char_idx)
{
    return (w[i] && 
           data->full_env_var_copy_gamma\
		   [data->env_var_line_idx][env_var_char_idx] \
		   &&
           w[i] == \
		   data->full_env_var_copy_gamma\
		   [data->env_var_line_idx][env_var_char_idx] \
		   && w[i] != '=');
}

bool	is_valid_env_var_entry(t_data *data)
{
    return (data->full_env_var_copy_gamma[++data->env_var_line_idx] != NULL);
}

bool	is_env_var_value_non_empty(t_data *data, int env_var_char_idx)
{
    return (data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx++] != '\0');
}

bool	is_env_var_name_with_equal_sign(t_data *data, int env_var_char_idx)
{
    return (data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx] \
	&& \
	data->full_env_var_copy_gamma\
	[data->env_var_line_idx][env_var_char_idx] == '=');
}