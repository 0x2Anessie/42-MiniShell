
// Definie la reaction du programme a un ctrl+c normal, et pendant un heredoc

#include "../../include/minishell.h"

unsigned int g_error_signal = 0;

void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_error_signal = 130;
}

void	ctrl_c_handler_here_doc(int sig)
{
	(void)sig;
	close(0);
	ft_putchar('\n');
	g_all.utils->is_here_doc = 0;
	g_all.utils->can_run = 0;
	g_error_signal = 130;
}

void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_error_signal = 130;
	ft_putchar('\n');
}

void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	g_error_signal = 131;
	write(1, "Quit\n", 5);
}

void	handle_sig(void)
{
	if (g_all.utils->is_here_doc)
	{
		signal(SIGINT, &ctrl_c_handler_here_doc);
	}
	else
	{
		signal(SIGINT, &ctrl_c_handler);
		signal(SIGQUIT, IGNORE_SIG_ACTION);
	}
}
