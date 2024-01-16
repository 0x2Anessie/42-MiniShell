#include "../../include/minishell.h"

bool	is_execve_failed(char *command_path, char **arguments, char **env)
{
	return (execve(command_path, arguments, env) == FAIL);
}
