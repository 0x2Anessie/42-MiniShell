#include "../../include/minishell.h"

/**
 * @nom: ctrl_c_handler
 *
 * @description:
 * Gestionnaire de signal pour CTRL-C (SIGINT) utilisé dans un contexte général 
 * du programme. Cette fonction définit la réponse du programme à l'interruption 
 * par CTRL-C, typiquement utilisée pour interrompre des opérations en cours et 
 * réinitialiser l'interface utilisateur.
 */
void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_globi = exit_stat_ctrl_c_sigint();
}

/**
 * @nom: ctrl_c_handler_here_doc
 *
 * @description:
 * Gestionnaire de signal pour le signal CTRL-C (SIGINT) spécifique
 * au contexte des 'here documents'. Cette fonction définit la manière
 * dont le programme doit réagir lorsque CTRL-C est pressé pendant la 
 * lecture d'un 'here document'.
 */
void	ctrl_c_handler_here_doc(int sig)
{
	(void)sig;
	close(0);
	ft_putchar('\n');
	g_globi = exit_stat_ctrl_c_sigint();
}

/**
 * @nom: handle_sig
 *
 * @description:
 * Configure les gestionnaires de signaux pour différents contextes d'exécution 
 * dans le programme. Cette fonction ajuste la réponse du programme aux signaux 
 * CTRL-C (SIGINT) et CTRL-Backslash (SIGQUIT) en fonction de l'état actuel, 
 * particulièrement en ce qui concerne le traitement des 'here documents'.
 */
void	handle_sig(t_data *data)
{
	if (data->utils->is_this_an_exec_in_heredoc)
	{	
		signal(SIGINT, &ctrl_c_handler_here_doc);
		if (g_globi == 130)
		{
			data->utils->is_this_an_exec_in_heredoc = 0;
			data->utils->heredoc_ctrl_c_uninterrupted = 0;
		}
	}
	else
	{
		signal(CTRL_C_SIGINT, &ctrl_c_handler);
		signal(CTRL_BACKSLSH, IGNORE_SIG_ACTION);
	}
}
