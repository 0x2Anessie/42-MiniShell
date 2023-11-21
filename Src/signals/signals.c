/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:32:56 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/21 16:14:54 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/minishell.h"

/*
	C'est le gestionnaire pour SIGINT, elle met a jour le code d'erreur a
	130 et renvoie une nouvelle ligne.
*/
static void	sigint_handler(int sig)
{
	(void)sig;
	g_all.utils->err = 130;
	ft_putchar('\n');
}

/*
	Gestionnaire de signaux pour SIGQUIT.
	Definie le code d'erreur a 131, qui est habituellement associe a
	l'interruption d'un processus par le signal SIGQUIT.
*/
static void	sigquit_handler(int sig)
{
	(void)sig;
	g_all.utils->err = 131;
	write(2, "Quit\n", 5);
}

/*
	Set des gestionnaires pour SIGINT et SIGQUIT.
	signal(le signal recu, &le handler qui va le gerer)
*/
void	handle_process_signal(void)
{
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, &sigquit_handler);
}
