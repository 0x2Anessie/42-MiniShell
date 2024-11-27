#include "../../include/minishell.h"

int	out_to_file_flags(void)
{
	return (O_CREAT | O_RDWR | O_TRUNC);
}

int	append_to_file_flags(void)
{
	return (O_CREAT | O_RDWR | O_APPEND);
}

int	heredoc_tmp_file_flags(void)
{
	return (O_CREAT | O_WRONLY | O_TRUNC);
}
