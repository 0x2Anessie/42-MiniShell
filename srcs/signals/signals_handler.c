/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabeaude <pabeaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:21:53 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:03:45 by pabeaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_globi = 130;
}

void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	g_globi = 131;
	write(2, "Quit\n", 5);
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
