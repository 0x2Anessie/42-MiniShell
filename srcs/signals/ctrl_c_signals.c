#include "../../include/minishell.h"

/**
 * @nom: ctrl_c_handler
 *
 * @description:
 * Gestionnaire de signal pour CTRL-C (SIGINT) utilisé dans un contexte général 
 * du programme. Cette fonction définit la réponse du programme à l'interruption 
 * par CTRL-C, typiquement utilisée pour interrompre des opérations en cours et 
 * réinitialiser l'interface utilisateur.
 *
 * @pourquoi:
 * - Réponse standard à l'interruption : Le traitement de CTRL-C dans ce
 * gestionnaire est destiné à des situations ordinaires, hors des 'here
 * documents' ou d'autres contextes spéciaux. Cela permet une gestion
 * cohérente et prévisible des interruptions.
 * - Réinitialisation de l'interface utilisateur : L'utilisation de fonctions
 * comme rl_replace_line et rl_redisplay aide à remettre à zéro la ligne de
 * commande et à rafraîchir l'affichage, offrant ainsi une interface claire et
 * prête pour de nouvelles entrées après l'interruption.
 * - Communication visuelle avec l'utilisateur : L'affichage d'un nouveau
 * retour à la ligne indique visuellement à l'utilisateur que son action
 * d'interruption a été reconnue et que le programme est prêt pour une
 * nouvelle commande.
 *
 * @parametres:
 * - sig: Le signal reçu, dans ce cas SIGINT (ignoré dans cette fonction).
 *
 * @fonctionnement:
 * - Ignore le paramètre 'sig'.
 * - Affiche un retour à la ligne (NEWLINE) sur la sortie standard.
 * - Réinitialise la ligne de commande avec rl_replace_line.
 * - Rafraîchit l'affichage de la ligne de commande avec rl_redisplay.
 * - Met à jour la variable 'err' dans la structure globale 'g_all.utils' à 
 * EXIT_STAT_CTRL_C_SIGINT.
 *
 * @valeur_de_retour:
 * Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 * - Les appels aux fonctions de réinitialisation et d'affichage de la ligne de 
 * commande pourraient échouer, mais la fonction ne gère pas ces cas d'erreur.
 *
 * @exemples_utilisation:
 * - Configurée comme gestionnaire de signal pour SIGINT dans des contextes 
 * généraux du programme, en dehors de situations spécifiques comme les
 * 'here documents'.
 *
 * @dependances:
 * - g_all: Structure globale contenant l'état du programme.
 * - ft_putchar: Fonction pour écrire un caractère sur la sortie standard.
 * - rl_on_new_line, rl_replace_line, rl_redisplay: Fonctions de la bibliothèque 
 * readline pour gérer l'affichage de la ligne de commande.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Écrire NEWLINE sur la sortie standard
 *   |
 *   v
 * Réinitialiser et rafraîchir la ligne de commande
 *   |
 *   v
 * Mettre à jour g_all.utils->err à EXIT_STAT_CTRL_C_SIGINT
 *   |
 *   v
 * Fin
 */
void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	globi = exit_stat_ctrl_c_sigint();
}

/**
 * @nom: ctrl_c_handler_here_doc
 *
 * @description:
 * Gestionnaire de signal pour le signal CTRL-C (SIGINT) spécifique au contexte 
 * des 'here documents'. Cette fonction définit la manière dont le programme
 * doit réagir lorsque CTRL-C est pressé pendant la lecture d'un
 * 'here document'.
 *
 * @pourquoi:
 * - Gestion spécifique pour les 'here documents' : Dans le contexte d'un
 * 'here document', l'interruption par CTRL-C nécessite un traitement
 * particulier, comme la fermeture de l'entrée standard pour arrêter la
 * lecture.
 * - Restauration de l'état du programme : La mise à jour des variables de la 
 * structure 'g_all.utils' assure que le programme sort correctement de l'état 
 * 'here document' et ajuste son comportement en conséquence pour les 
 * opérations futures.
 * - Communication et feedback à l'utilisateur : L'écriture d'un retour à la
 * ligne sur STDERR après l'interruption donne un feedback visuel à 
 * l'utilisateur, indiquant que son action a été reconnue et traitée.
 *
 * @parametres:
 * - sig: Le signal reçu, dans ce cas SIGINT (ignoré dans cette fonction).
 *
 * @fonctionnement:
 * - Ignore le paramètre 'sig'.
 * - Ferme l'entrée standard (file descriptor 0) pour interrompre la lecture du 
 * 'here document'.
 * - Écrit un retour à la ligne (FT_NEWLINE) sur STDERR.
 * - Met à jour les variables dans la structure globale 'g_all.utils', notamment 
 * 'is_here_doc' à 0, 'can_run' à 0 et 'err' à EXIT_STAT_CTRL_C_SIGINT.
 *
 * @valeur_de_retour:
 * Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 * - La fermeture de l'entrée standard et l'écriture sur STDERR pourraient 
 * échouer, mais la fonction ne gère pas ces cas d'erreur.
 *
 * @exemples_utilisation:
 * - Configurée comme gestionnaire de signal pour SIGINT pendant la lecture d'un 
 * 'here document'.
 *
 * @dependances:
 * - g_all: Structure globale contenant l'état du programme.
 * - close: Fonction de la bibliothèque standard pour fermer un descripteur de
 * fichier.
 * - write: Fonction pour écrire dans un descripteur de fichier.
 * - ft_strlen: Fonction pour calculer la longueur d'une chaîne de caractères.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * Fermer l'entrée standard
 *   |
 *   v
 * Écrire FT_NEWLINE sur STDERR
 *   |
 *   v
 * Mettre à jour g_all.utils (is_here_doc, can_run, err)
 *   |
 *   v
 * Fin
 */
void	ctrl_c_handler_here_doc(int sig, t_data *data)
{
	(void)sig;
	close(0);
	ft_putchar('\n');
	data->utils->is_here_doc = 0;
	data->utils->can_run = 0;
	globi = exit_stat_ctrl_c_sigint();
}

/**
 * @nom: handle_sig
 *
 * @description:
 * Configure les gestionnaires de signaux pour différents contextes d'exécution 
 * dans le programme. Cette fonction ajuste la réponse du programme aux signaux 
 * CTRL-C (SIGINT) et CTRL-Backslash (SIGQUIT) en fonction de l'état actuel, 
 * particulièrement en ce qui concerne le traitement des 'here documents'.
 *
 * @pourquoi:
 * - Adaptation contextuelle : Les besoins en matière de gestion des signaux 
 * peuvent varier selon l'état du programme. Par exemple, pendant la lecture
 * d'un 'here document', la réaction à CTRL-C pourrait différer de celle en
 * mode normal.
 * - Prévention des comportements par défaut : En redéfinissant les
 * gestionnaires de signaux, le programme évite les réponses par défaut du
 * système (comme la fermeture du programme), permettant une expérience
 * utilisateur plus riche et contrôlée.
 * - Flexibilité dans la gestion des signaux : Permet au programme de changer 
 * dynamiquement les gestionnaires en fonction de l'état d'exécution, offrant 
 * ainsi une plus grande flexibilité et une meilleure réponse aux actions de 
 * l'utilisateur.
 *
 * @parametres:
 * Aucun paramètre d'entrée.
 *
 * @fonctionnement:
 * - Vérifie l'état 'is_here_doc' dans la structure globale 'g_all.utils'.
 * - Si 'is_here_doc' est vrai, configure le signal CTRL_C_SIGINT pour utiliser
 * le gestionnaire ctrl_c_handler_here_doc, spécifique au contexte 
 * 'here document'.
 * - Si 'is_here_doc' est faux, configure le signal CTRL_C_SIGINT pour utiliser
 * le gestionnaire ctrl_c_handler standard et le signal CTRL_BACKSLSH pour 
 * ignorer l'action (IGNORE_SIG_ACTION).
 *
 * @valeur_de_retour:
 * Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 * - L'installation des gestionnaires de signaux peut échouer, mais cette
 * fonction ne gère pas ces éventualités.
 *
 * @exemples_utilisation:
 * - Appelée pour configurer les gestionnaires de signaux en fonction du
 * contexte, notamment lors de l'entrée et de la sortie des 'here documents'.
 *
 * @dependances:
 * - g_all: Structure globale contenant l'état du programme.
 * - signal: Fonction standard C pour la gestion des signaux.
 * - ctrl_c_handler_here_doc: Gestionnaire spécifique pour SIGINT dans le
 * contexte 'here document'.
 * - ctrl_c_handler: Gestionnaire standard pour SIGINT.
 * - IGNORE_SIG_ACTION: Constante définissant l'action d'ignorer un signal.
 *
 * @graphe_de_flux:
 * Début
 *   |
 *   v
 * g_all.utils->is_here_doc est vrai ?
 *   /        \
 * VRAI      FAUX
 *   |          \
 *   |           \
 *   v            v
 * Configurer     Configurer CTRL_C_SIGINT avec ctrl_c_handler
 * CTRL_C_SIGINT avec    Configurer CTRL_BACKSLSH avec IGNORE_SIG_ACTION
 * ctrl_c_handler_here_doc
 *   |
 *   v
 * Fin
 */
void	handle_sig(t_data *data)
{
	if (data->utils->is_here_doc)/*         ---> condition non intelligible --> fonction         */
		signal(SIGINT, (void (*)(int))ctrl_c_handler_here_doc);
	else
	{
		signal(CTRL_C_SIGINT, &ctrl_c_handler);
		signal(CTRL_BACKSLSH, IGNORE_SIG_ACTION);
	}
}
