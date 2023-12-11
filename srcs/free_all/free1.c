
#include "../../include/minishell.h"

int	ft_exit_child(t_exec *utils, int *fd, t_data *data)
{
	int	k;

	k = utils->err;
	if (fd)
		close_pipe(fd);
	close_fd();
	ft_free_all(data);
	exit(k);
}

void	check_memory_allocation(t_data *data, void *add)
{
	t_memory	*mem;
	t_memory	*tmp;

	mem = malloc(sizeof(t_memory));
	if (!mem)
		return ;
	mem->add = add;
	mem->next = NULL;
	if (data->memory == NULL)
		data->memory = mem;
	else
	{
		tmp = data->memory;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = mem;
	}	
}

void	*ft_malloc_with_tracking(t_data *data, size_t size)
{
	void	*add;

	add = malloc(size);
	if (!add)
		return (NULL);
	check_memory_allocation(data, add);
	return (add);
}

void	ft_free(void *add, t_data *data)
{
	t_memory	*tmp;
	t_memory	*ex;
	t_memory	*next;

	tmp = data->memory;
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
				data->memory = next;
			return ;
		}
		ex = tmp;
		tmp = tmp->next;
	}
}

void	ft_free_all(t_data *data)
{
	t_memory	*tmp;
	t_memory	*next;

	tmp = data->memory;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->add)
			free(tmp->add);
		free(tmp);
		tmp = next;
	}
}
