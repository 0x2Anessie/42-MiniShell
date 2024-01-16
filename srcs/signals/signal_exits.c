#include "../../include/minishell.h"

int	exit_stat_ctrl_backslash_sigquit(void)
{
	return (SIGNAL_EXIT_OFFSET + SIGQUIT);
}

int	exit_stat_ctrl_c_sigint(void)
{
	return (SIGNAL_EXIT_OFFSET + SIGINT);
}
