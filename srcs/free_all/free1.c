/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:56:20 by fililafrapp       #+#    #+#             */
/*   Updated: 2023/11/30 01:47:53 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit_child(t_exec *utils, int *fd)
{
	int	k;

	k = utils->err;
	if (fd)
		close_pipe(fd);
	close_fd();
	ft_free_all();
	exit(k);
}

void	check_memory_allocation(void *add)
{
	t_memory	*mem;
	t_memory	*tmp;

	mem = malloc(sizeof(t_memory));
	if (!mem)
		return ;
	mem->add = add;
	mem->next = NULL;
	if (g_all.d_mem == NULL)
		g_all.d_mem = mem;
	else
	{
		tmp = g_all.d_mem;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = mem;
	}	
}

void	*ft_malloc_with_tracking(size_t size)
{
	void	*add;

	add = malloc(size);
	if (!add)
		return (NULL);
	check_memory_allocation(add);
	return (add);
}

void	ft_free(void *add)
{
	t_memory	*tmp;
	t_memory	*ex;
	t_memory	*next;

	tmp = g_all.d_mem;
	ex = NULL;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->add == add)
		{
			tmp->add = NULL;
			free(tmp->add);
			free(tmp);
			if (ex)
				ex->next = next;
			else
				g_all.d_mem = next;
			return ;
		}
		ex = tmp;
		tmp = tmp->next;
	}
}

void	ft_free_all(void)
{
	t_memory	*tmp;
	t_memory	*next;

	tmp = g_all.d_mem;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->add)
			free(tmp->add);
		free(tmp);
		tmp = next;
	}
}
