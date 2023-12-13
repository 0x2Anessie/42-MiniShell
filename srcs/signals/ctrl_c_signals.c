// Definie la reaction du programme a un ctrl+c normal, et pendant un heredoc

#include "../../include/minishell.h"

void	ctrl_c_handler(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	globi = 130;
}

void	ctrl_c_handler_here_doc(int sig, t_data *data)
{
	(void)sig;
	close(0);
	ft_putchar('\n');
	data->utils->is_here_doc = 0;
	data->utils->can_run = 0;
	globi = 130;
}

void	handle_ctrl_c(int sig)
{
	(void)sig;
	globi = 130;
	ft_putchar('\n');
}

void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	globi = 131;
	write(1, "Quit\n", 5);
}

void	handle_sig(t_data *data)
{
	if (data->utils->is_here_doc)
	{
		//(void)sig;
		close(0);
		ft_putchar('\n');
		data->utils->is_here_doc = 0;
		data->utils->can_run = 0;
		globi = 130;
		//signal(SIGINT, &ctrl_c_handler_here_doc);
	}
	else
	{
		signal(SIGINT, &ctrl_c_handler);
		signal(SIGQUIT, IGNORE_SIG_ACTION);
	}
}
