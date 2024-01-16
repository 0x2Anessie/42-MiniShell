#include "../../include/minishell.h"

bool	is_pipe_creation_failed(int *fd, int total_number_of_cmd)
{
	return (total_number_of_cmd >= 1 && pipe(fd) < 0);
}

bool	is_single_command_in_list(t_data *data)
{
	return (data->utils->total_number_of_cmd_find_in_linked_list == 1);
}

bool	is_single_builtin_command(t_data *data)
{
	return (\
	is_built_in_command(data->lexer_list) && is_single_command_in_list(data));
}
