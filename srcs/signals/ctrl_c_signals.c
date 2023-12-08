
// Definie la reaction du programme a un ctrl+c normal, et pendant un heredoc

#include "../../include/minishell.h"

/**
 * @nom: ctrl_c_handler
 *
 * @description:
 *   Gestionnaire de signal pour CTRL-C (SIGINT) utilisé dans un contexte général 
 *   du programme. Cette fonction définit la réponse du programme à l'interruption 
 *   par CTRL-C, typiquement utilisée pour interrompre des opérations en cours et 
 *   réinitialiser l'interface utilisateur.
 *
 * @pourquoi:
 *   - Réponse standard à l'interruption : Le traitement de CTRL-C dans ce gestionnaire 
 *     est destiné à des situations ordinaires, hors des 'here documents' ou d'autres 
 *     contextes spéciaux. Cela permet une gestion cohérente et prévisible des 
 *     interruptions.
 *   - Réinitialisation de l'interface utilisateur : L'utilisation de fonctions comme 
 *     rl_replace_line et rl_redisplay aide à remettre à zéro la ligne de commande et 
 *     à rafraîchir l'affichage, offrant ainsi une interface claire et prête pour de 
 *     nouvelles entrées après l'interruption.
 *   - Communication visuelle avec l'utilisateur : L'affichage d'un nouveau retour à 
 *     la ligne indique visuellement à l'utilisateur que son action d'interruption a 
 *     été reconnue et que le programme est prêt pour une nouvelle commande.
 */
void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar(NEWLINE);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_all.utils->err = CTRL_C_ERROR;
}

/**
 * @nom: ctrl_c_handler_here_doc
 *
 * @description:
 *   Gestionnaire de signal pour le signal CTRL-C (SIGINT) spécifique au contexte 
 *   des 'here documents'. Cette fonction définit la manière dont le programme doit 
 *   réagir lorsque CTRL-C est pressé pendant la lecture d'un 'here document'.
 *
 * @pourquoi:
 *   - Gestion spécifique pour les 'here documents' : Dans le contexte d'un 'here 
 *     document', l'interruption par CTRL-C nécessite un traitement particulier, 
 *     comme la fermeture de l'entrée standard pour arrêter la lecture.
 *   - Restauration de l'état du programme : La mise à jour des variables de la 
 *     structure 'g_all.utils' assure que le programme sort correctement de l'état 
 *     'here document' et ajuste son comportement en conséquence pour les 
 *     opérations futures.
 *   - Communication et feedback à l'utilisateur : L'écriture d'un retour à la ligne 
 *     sur STDERR après l'interruption donne un feedback visuel à l'utilisateur, 
 *     indiquant que son action a été reconnue et traitée.
 */
void	ctrl_c_handler_here_doc(int sig)
{
	(void)sig;
	close(0);
	ft_putchar(NEWLINE);
	g_all.utils->is_here_doc = 0;
	g_all.utils->can_run = 0;
	g_all.utils->err = CTRL_C_ERROR;
}

/**
 * @nom: handle_sig
 *
 * @description:
 *   Configure les gestionnaires de signaux pour différents contextes d'exécution 
 *   dans le programme. Cette fonction ajuste la réponse du programme aux signaux 
 *   CTRL-C (SIGINT) et CTRL-Backslash (SIGQUIT) en fonction de l'état actuel, 
 *   particulièrement en ce qui concerne le traitement des 'here documents'.
 *
 * @pourquoi:
 *   - Adaptation contextuelle : Les besoins en matière de gestion des signaux 
 *     peuvent varier selon l'état du programme. Par exemple, pendant la lecture d'un 
 *     'here document', la réaction à CTRL-C pourrait différer de celle en mode normal.
 *   - Prévention des comportements par défaut : En redéfinissant les gestionnaires 
 *     de signaux, le programme évite les réponses par défaut du système (comme la 
 *     fermeture du programme), permettant une expérience utilisateur plus riche et 
 *     contrôlée.
 *   - Flexibilité dans la gestion des signaux : Permet au programme de changer 
 *     dynamiquement les gestionnaires en fonction de l'état d'exécution, offrant 
 *     ainsi une plus grande flexibilité et une meilleure réponse aux actions de 
 *     l'utilisateur.
 */
void	handle_sig(void)
{
	if (g_all.utils->is_here_doc)
		signal(SIGINT, &ctrl_c_handler_here_doc);
	else
	{
		signal(SIGINT, &ctrl_c_handler);
		signal(SIGQUIT, IGNORE_SIG_ACTION);
	}	
}
