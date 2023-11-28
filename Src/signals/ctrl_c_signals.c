/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 13:29:26 by fd-arco           #+#    #+#             */
/*   Updated: 2023/11/22 12:34:03 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Definie la reaction du programme a un ctrl+c normal, et pendant un heredoc

#include "../../Include/minishell.h"

/*
	Simule un ctrl+c en affichant l'invite de commande a la ligne.
	Réinitialise la ligne de commande actuelle pour readline.
	Met le code d'erreur a 130, la sortie standart pour un process interrompu
	par SIGINT.
*/
void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_all.utils->err = 130;
}

/*
	Simule un ctrl+c dans le cas où un heredoc est en cours de traitement.
	Ferme le descripteur de fichier, affiche une nouvelle ligne et met que
	la commande ne peut plus d'executer.
	Met le code d'erreur a 130.
*/
void	ctrl_c_handler_heredoc(int sig)
{
	(void)sig;
	close(0);
	write(2, "\n", 1);
	g_all.utils->is_here_doc = 0;
	g_all.utils->can_run = 0;
	g_all.utils->err = 130;
}

/*
	Set 'ctrl_c_handler_here_doc' et 'ctrl_c_handler' comme gestionnaires de
	signaux pour SIGINT.
*/
void	handle_sig(void)
{
	if (g_all.utils->is_here_doc)
		signal(SIGINT, &ctrl_c_handler_heredoc);
	else
	{
		signal(SIGINT, &ctrl_c_handler);
		signal(SIGQUIT, SIG_IGN);
	}	
}
