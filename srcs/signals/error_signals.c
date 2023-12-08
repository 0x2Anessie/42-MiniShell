// Definie les retours d'erreur pour les signaux SIGINT et SIGQUIT

#include "../../include/minishell.h"

/**
 * @nom: handle_ctrl_c
 *
 * @description:
 *   Gestionnaire de signal pour CTRL-C (SIGINT). Cette fonction définit la réaction 
 *   du programme lorsqu'un signal d'interruption (SIGINT) est envoyé, typiquement 
 *   utilisé pour interrompre l'exécution d'un processus en cours.
 *
 * @pourquoi:
 *   - Gestion des interruptions utilisateur : La capture et la gestion de SIGINT 
 *     permettent au programme de réagir de manière appropriée à une demande 
 *     d'interruption par l'utilisateur. Cela peut impliquer l'arrêt propre d'une 
 *     opération en cours, la sauvegarde de l'état, ou simplement l'indication que 
 *     l'interruption a été reconnue.
 *   - Feedback visuel à l'utilisateur : L'affichage d'un retour à la ligne (NEWLINE) 
 *     après une interruption donne un feedback visuel immédiat, indiquant que le 
 *     signal a été reçu et traité.
 *   - Préservation du contexte d'erreur : La mise à jour de la variable 'err' dans 
 *     la structure 'g_all.utils' permet de garder une trace de l'interruption pour 
 *     une éventuelle gestion ultérieure des erreurs ou pour des fins de débogage.
 */
static void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_all.utils->err = CTRL_C_ERROR;
	ft_putchar(NEWLINE);
}

/**
 * @nom: handle_ctrl_backslash
 *
 * @description:
 *   Gestionnaire de signal pour CTRL-Backslash (SIGQUIT). Cette fonction définit la 
 *   réaction du programme lorsque l'utilisateur envoie le signal de terminaison 
 *   forcée (SIGQUIT), généralement utilisé pour arrêter immédiatement un processus.
 *
 * @pourquoi:
 *   - Contrôle des terminaisons forcées : La gestion de SIGQUIT permet au programme 
 *     de répondre de manière contrôlée à une demande de terminaison forcée. Cela 
 *     peut être utilisé pour assurer une fermeture propre, enregistrer l'état actuel 
 *     ou afficher un message spécifique.
 *   - Communication avec l'utilisateur : En affichant un message personnalisé lors 
 *     de la réception de SIGQUIT, le programme informe l'utilisateur que la demande 
 *     de terminaison a été reçue et traitée.
 *   - Préservation de l'état du programme : En ajustant la variable 'err' de la 
 *     structure 'g_all.utils', le programme peut garder une trace de la manière dont 
 *     il a été terminé, ce qui peut être utile pour le débogage ou la gestion des 
 *     erreurs.
 */
static void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	g_all.utils->err = CTRL_BACKSLASH_ERROR;
	write(1, "Quit\n", 5);
}

/**
 * @nom: handle_process_signal
 *
 * @description:
 *   Configure les gestionnaires de signaux pour le processus en cours. Cette 
 *   fonction est essentielle dans des applications interactives comme les shells ou 
 *   les éditeurs de texte pour personnaliser la réponse aux interruptions et terminaisons.
 *
 * @pourquoi:
 *   - Personnalisation de la gestion des signaux : Les signaux comme CTRL-C et 
 *     CTRL-Backslash ont des comportements par défaut dans les programmes. En les 
 *     gérant spécifiquement, le programme peut réagir de manière plus appropriée 
 *     et contrôlée, par exemple, en nettoyant les ressources ou en affichant un message 
 *     personnalisé.
 *   - Amélioration de l'expérience utilisateur : Dans un shell, intercepter CTRL-C 
 *     permet d'interrompre une commande en cours sans fermer le shell lui-même, 
 *     améliorant ainsi l'interaction de l'utilisateur avec le shell.
 *   - Sécurité et stabilité : La gestion personnalisée des signaux contribue à la 
 *     stabilité de l'application en prévenant les terminaisons abruptes et en assurant 
 *     une fermeture propre des processus.
 */
void	handle_process_signal(void)
{
	signal(SIGINT, &handle_ctrl_c);
	signal(SIGQUIT, &handle_ctrl_backslash);
}
