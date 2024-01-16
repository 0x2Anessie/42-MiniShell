#include "../../include/minishell.h"

int	ft_exit_child(int *fd, t_data *data)
{

	if (fd)
		closes_ends_of_pipe(fd);
	close_file_descriptors_in_linked_list(data);
	ft_free_all(data);
	exit(0);
}

// int	ft_exit_child(t_exec *utils, int *fd, t_data *data)
// {
// 	int	k;

// 	k = utils->err;
// 	if (fd)
// 		closes_ends_of_pipe(fd);
// 	close_file_descriptors_in_linked_list(data);
// 	ft_free_all(data);
// 	exit(k);
// }

void	add_to_trash_memory(t_data *data, void *add)
{
	t_trash_memory	*mem;
	t_trash_memory	*tmp;

	mem = malloc(sizeof(t_trash_memory));
	if (!mem)
		return ;
	mem->add = add;
	mem->next = NULL;
	if (data->trash_memory == NULL)
		data->trash_memory = mem;
	else
	{
		tmp = data->trash_memory;
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
	add_to_trash_memory(data, add);
	return (add);
}

void	ft_free(void *add, t_data *data)
{
	t_trash_memory	*tmp;
	t_trash_memory	*ex;
	t_trash_memory	*next;

	tmp = data->trash_memory;
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
				data->trash_memory = next;
			return ;
		}
		ex = tmp;
		tmp = tmp->next;
	}
}

void	ft_free_all(t_data *data)
{
	t_trash_memory	*tmp;
	t_trash_memory	*next;

	tmp = data->trash_memory;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->add)
			free(tmp->add);
		free(tmp);
		tmp = next;
	}
}
