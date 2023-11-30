/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:32:56 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/30 02:20:41 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 *
 * @paramètres:
 *   - sig: Le signal reçu, dans ce cas SIGINT (ignoré dans cette fonction).
 *
 * @fonctionnement:
 *   - Ignore le paramètre 'sig' (pour respecter la signature standard des gestionnaires 
 *     de signal).
 *   - Met à jour la variable 'err' de la structure globale 'g_all.utils' pour indiquer 
 *     que le programme a reçu un signal SIGINT.
 *   - Affiche un caractère de nouvelle ligne (NEWLINE) sur la sortie standard.
 *
 * @valeur_de_retour:
 *   Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 *   - L'écriture du caractère de nouvelle ligne pourrait échouer, mais il n'y a pas 
 *     de gestion d'erreur spécifique dans cette fonction.
 *
 * @exemples_d'utilisation:
 *   - Configuré comme gestionnaire de signal pour SIGINT dans un programme qui 
 *     nécessite une gestion personnalisée des interruptions utilisateur.
 *
 * @dépendances:
 *   - g_all: Structure globale contenant l'état du programme.
 *   - ft_putchar: Fonction pour écrire un caractère sur la sortie standard.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Mise à jour de g_all.utils->err avec EXIT_STAT_CTRL_C_SIGINT
 *     |
 *     v
 *   Écriture de NEWLINE sur la sortie standard
 *     |
 *     v
 *   Fin
 */
static void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_all.utils->err = EXIT_STAT_CTRL_C_SIGINT;
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
 *
 * @paramètres:
 *   - sig: Le signal reçu, dans ce cas SIGQUIT (ignoré dans cette fonction).
 *
 * @fonctionnement:
 *   - Ignore le paramètre 'sig' (utilisé pour respecter la signature standard des 
 *     gestionnaires de signal).
 *   - Met à jour la variable 'err' de la structure globale 'g_all.utils' pour indiquer 
 *     que le programme a reçu un signal SIGQUIT.
 *   - Affiche un message QUIT_MESSAGE sur STDERR.
 *
 * @valeur_de_retour:
 *   Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 *   - L'écriture sur STDERR pourrait échouer, mais il n'y a pas de gestion d'erreur 
 *     pour ce cas dans cette fonction.
 *
 * @exemples_d'utilisation:
 *   - Configuré comme gestionnaire de signal pour SIGQUIT dans le cadre d'un 
 *     interpréteur de commandes ou d'un programme similaire.
 *
 * @dépendances:
 *   - g_all: Structure globale contenant l'état du programme.
 *   - write: Fonction de la bibliothèque standard pour écrire dans un descripteur 
 *     de fichier.
 *   - ft_strlen: Fonction pour calculer la longueur d'une chaîne de caractères.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Mise à jour de g_all.utils->err avec EXIT_STAT_CTRL_BACKSLSH_SIGQUIT
 *     |
 *     v
 *   Écriture de QUIT_MESSAGE sur STDERR
 *     |
 *     v
 *   Fin
 */
static void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	g_all.utils->err = EXIT_STAT_CTRL_BACKSLSH_SIGQUIT;
	write(STDERR_FILENO, QUIT_MESSAGE, ft_strlen(QUIT_MESSAGE)) ;
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
 *
 * @paramètres:
 *   Aucun paramètre d'entrée.
 *
 * @fonctionnement:
 *   - Appelle la fonction signal pour CTRL_C, en lui associant le gestionnaire 
 *     handle_ctrl_c.
 *   - Appelle la fonction signal pour CTRL_BACKSLSH, en lui associant le gestionnaire 
 *     handle_ctrl_backslash.
 *
 * @valeur_de_retour:
 *   Aucune valeur de retour (fonction void).
 *
 * @erreurs_et_effets_de_bord:
 *   - Si l'installation des gestionnaires de signaux échoue, le comportement par 
 *     défaut pour les signaux concernés reste en place.
 *
 * @exemples_d'utilisation:
 *   - Appelée au début d'un programme pour configurer les réactions personnalisées 
 *     aux signaux d'interruption et de terminaison.
 *
 * @dépendances:
 *   - signal: Fonction standard C pour la gestion des signaux.
 *   - handle_ctrl_c: Gestionnaire pour le signal CTRL-C.
 *   - handle_ctrl_backslash: Gestionnaire pour le signal CTRL-Backslash.
 *
 * @graphe_de_flux:
 *   Début
 *     |
 *     v
 *   Configurer signal pour CTRL_C avec handle_ctrl_c
 *     |
 *     v
 *   Configurer signal pour CTRL_BACKSLSH avec handle_ctrl_backslash
 *     |
 *     v
 *   Fin
 */
void	handle_process_signal(void)
{
	signal(CTRL_C, &handle_ctrl_c);
	signal(CTRL_BACKSLSH, &handle_ctrl_backslash);
}
