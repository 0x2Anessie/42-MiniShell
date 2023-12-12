// Definie les retours d'erreur pour les signaux SIGINT et SIGQUIT

#include "../../include/minishell.h"

/**
 * @description:
 *   Configure les gestionnaires de signaux pour le processus en cours. Cette 
 *   fonction est essentielle dans des applications interactives comme les shells ou 
 *   les éditeurs de texte pour personnaliser la réponse aux interruptions et terminaisons.
 */
void	handle_process_signal(void)
{
	signal(SIGINT, &handle_ctrl_c);
	signal(SIGQUIT, &handle_ctrl_backslash);
}
