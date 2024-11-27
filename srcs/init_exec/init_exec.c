#include "../../include/minishell.h"

t_node	*insert_command_at_end_of_linked_list(t_node *node_lst, t_node *new)
{
	t_node	*tmp;

	tmp = node_lst;
	if (!tmp)
		return (new);
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (node_lst);
}

bool	is_there_more_cmds_to_exec(int index, int total_cmds)
{
	return (index < total_cmds);
}

void	build_cmd_linked_list(t_node *node, t_data *data, t_exec *utils)
{
	int	index;

	index = ZERO_INIT;
	while (is_there_more_cmds_to_exec(index, utils->cmd_nb_pipe))
	{
		node = malloc_track(data, sizeof(t_node));
		if (!node)
			return ;
		setup_input_redirection(node, data->lexer_list, data);
		setup_output_redirection(node, data->lexer_list);
		node->next = NULL;
		node->is_command_present = is_token_type_cmd(data->lexer_list);
		utils->node = insert_command_at_end_of_linked_list(utils->node, node);
		while (is_current_token_not_pipe(data->lexer_list))
			data->lexer_list = data->lexer_list->next;
		if (data->lexer_list)
			data->lexer_list = data->lexer_list->next;
		index++;
	}
	data->lexer_list = utils->head_lexer_lst;
}

bool	is_env_var_list_uninitialized(t_exec *utils)
{
	return (utils->head_of_linked_list_env_var == NULL);
}

void	ft_init_exec(t_data *data)
{
	t_node	*node;

	data->utils->node = NULL;
	data->utils->is_this_an_exec_in_heredoc = 1;
	data->utils->heredoc_ctrl_c_uninterrupted = 1;
	data->utils->heredoc_nbr = 0;
	data->utils->heredoc_input_buffer = NULL;
	data->utils->nb_cmd_in_lst = \
	count_cmd_in_lexer_linked_list(data->lexer_list);
	data->utils->cmd_nb_pipe = \
	count_pipe_or_end_in_lexer_linked_list(data->lexer_list);
	data->utils->head_lexer_lst = data->lexer_list;
	data->utils->full_env_var_copy_beta = data->full_env_var_copy_alpha;
	if (is_env_var_list_uninitialized(data->utils))
	{
		data->utils->head_of_linked_list_env_var = \
		ft_buil_sorted_linked_list_env_var(\
		data, data->utils->head_of_linked_list_env_var);
		export_quotes(data->utils->head_of_linked_list_env_var, data);
	}
	node = NULL;
	build_cmd_linked_list(node, data, data->utils);
	data->utils->is_this_an_exec_in_heredoc = ZERO_INIT;
	handle_sig(data);
	data->utils->head_cmd_lst = data->utils->node;
}
