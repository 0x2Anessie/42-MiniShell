#include "../../include/minishell.h"

void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_globi = exit_stat_ctrl_c_sigint();
	ft_putchar(NEWLINE);
}

void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	g_globi = exit_stat_ctrl_backslash_sigquit();
	write(STDERR_FILENO, QUIT_MESSAGE, ft_strlen(QUIT_MESSAGE));
}

/**
 * @description:
 * Configure les gestionnaires de signaux pour le processus en cours. Cette 
 * fonction est essentielle dans des applications interactives comme les shells
 * ou  les éditeurs de texte pour personnaliser la réponse aux interruptions et
 * terminaisons.
 */
void	handle_process_signal(void)
{
	signal(SIGINT, &handle_ctrl_c);
	signal(SIGQUIT, &handle_ctrl_backslash);
}
